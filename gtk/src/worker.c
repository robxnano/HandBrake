/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright (C) 2023 HandBrake Team */

#define G_LOG_DOMAIN "hb-worker"

#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>

#include "server.h"

static GSocket *socket;
static hb_handle_t *h_job;
static hb_state_t *h_state;

static gboolean worker_update_status(GMainLoop *loop);

// Pause job
static void
worker_pause (void)
{
    hb_pause(h_job);
}

// Start or resume job
static void
worker_start (void)
{
    ghb_log("hb-worker: Starting job");
    hb_start(h_job);
}

// Cancel job and exit
static void
worker_stop (void)
{
    hb_stop(h_job);
}

// Initialize worker with single job
static gboolean
worker_init (int verbosity)
{
    g_autoptr(GSocketAddress) addr;
    g_autofree char *path;
    g_autoptr(GError) error = NULL;

    path = ghb_get_socket_path();
    addr = g_unix_socket_address_new(path);
    socket = g_socket_new(G_SOCKET_FAMILY_UNIX, G_SOCKET_TYPE_STREAM,
                          G_SOCKET_PROTOCOL_DEFAULT, NULL);
    g_socket_connect(socket, addr, NULL, &error);
    if (error)
    {
        g_warning("Could not connect to %s: %s", path, error->message);
        return FALSE;
    }

    hb_global_init();
    h_job = hb_init(verbosity);
    h_state = g_malloc0(sizeof(hb_state_t));
    return TRUE;
}

static void
worker_shutdown (void)
{
    g_socket_shutdown(socket, TRUE, TRUE, NULL);
    g_object_unref(socket);
    socket = NULL;
    g_free(h_state);
    h_state = NULL;
    hb_close(&h_job);
    hb_global_close();
}

// Since the worker process can't do anything until it receives
// instructions, wait here until the job JSON is received through
// the socket.
// Returns: The job description in JSON format, or null if the
// connection times out.
static char *
worker_receive_json (void)
{
    g_return_val_if_fail(G_IS_SOCKET(socket), NULL);
    gboolean result;
    g_autoptr(GError) error = NULL;
    char *json = NULL;
    uint32_t magic = 0;
    ssize_t data_len = 0;
    GInputVector header_vec[] = {
        { &magic, sizeof(uint32_t) },
        { &data_len, sizeof(ssize_t) },
        { NULL },
    };

    // Wait for the server to send something before reading it
    result = g_socket_condition_timed_wait(socket, G_IO_IN, 1000 * 1000, NULL, &error);
    if (!result)
    {
        ghb_log("hb-worker: Socket not readable: %s", error->message);
        return NULL;
    }
    ssize_t bytes = g_socket_get_available_bytes(socket);
    if (bytes > JSON_LEN)
    {
        ssize_t recv = 0;
        g_debug("Reading data from socket (%zd bytes)", bytes);
        g_socket_receive_message(socket, NULL, header_vec, -1, NULL, 0,
                                 G_SOCKET_MSG_NONE, NULL, &error);
        if (error)
        {
            g_warning("Socket read error: %s", error->message);
            return NULL;
        }
        else if (magic != JSON_MAGIC)
        {
            g_warning("Receive data failed: incorrect packet header");
            return NULL;
        }
        json = g_malloc0(data_len + 1); // Trust nobody not even yourself
        recv = g_socket_receive(socket, json, data_len, NULL, &error);
        if (recv != data_len)
        {
            g_warning("Socket read error: %s", error->message);
            g_free(json);
            json = NULL;
        }
        else
        {
            g_debug("Data received (%zd bytes)", recv);
        }
        if (data_len != bytes - HEADER_LEN) // Shouldn't happen
        {
            ghb_log("hb-worker: Extra data left unread in socket");
        }
    }
    else
    {
        ghb_log("hb-worker: no data received");
    }
    // TODO: Validate JSON
    return g_steal_pointer(&json);
}

static GhbWorkerCommand
worker_get_command (void)
{
    g_autoptr(GError) error = NULL;
    GhbWorkerCommand command = CMD_NONE;
    uint32_t magic = 0;
    ssize_t data_len = 0;
    GInputVector command_vec[] = {
        { &magic, sizeof(uint32_t) },
        { &data_len, sizeof(ssize_t) },
        { &command, sizeof(GhbWorkerCommand) },
    };
    if (!g_socket_condition_check(socket, G_IO_IN))
    {
        return CMD_NONE;
    }
    while (g_socket_get_available_bytes(socket) >= COMMAND_LEN)
    {
        g_socket_receive_message(socket, NULL, command_vec, 3, NULL, 0,
                                 G_SOCKET_MSG_NONE, NULL, &error);
        if (error)
        {
            ghb_log("hb-worker: Command receive error: %s", error->message);
            return CMD_NONE;
        }
        else if (magic != COMMAND_MAGIC)
        {
            ghb_log("hb-worker: Receive command failed: Incorrect packet header");
            return CMD_NONE;
        }
    }
    return command;
}

static gboolean
worker_send_state (hb_state_t *state)
{
    g_autoptr(GError) error = NULL;
    ssize_t data_len = 0;
    GOutputVector response_vec[] = {
        { &RESPONSE_MAGIC, sizeof(uint32_t) },
        { &data_len, sizeof(ssize_t) },
        { state, sizeof(hb_state_t) },
    };
    if (g_socket_condition_check(socket, G_IO_OUT))
    {
        g_socket_send_message(socket, NULL, response_vec, 3, NULL, 0,
                              G_SOCKET_MSG_NONE, NULL, &error);
        if (error)
        {
            g_warning("Could not send state: %s", error->message);
            return G_SOURCE_REMOVE;
        }
    }
    else
    {
        g_warning("socket was not ready to send data");
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}

/**
 * Get updates from libhb thread on the job progress and report
 * to the server. Then check for new instructions from the server,
 * and pause or terminate the job if necessary. Quits the loop if
 * the socket has closed
 * @loop: The worker's main loop
 * Returns: FALSE if the job has finished
 */
static gboolean
worker_update_status (GMainLoop *loop)
{
    if (!socket || !g_socket_is_connected(socket) || g_socket_is_closed(socket))
    {
        g_main_loop_quit(loop);
        return G_SOURCE_REMOVE;
    }
    hb_get_state(h_job, h_state);

    switch (h_state->state)
    {
        case HB_STATE_WORKING:
            break;
        case HB_STATE_WORKDONE:
            g_main_loop_quit(loop);
            return G_SOURCE_REMOVE;
        default:
            break;
    }
    return G_SOURCE_CONTINUE;
}

static gboolean
worker_socket_communicate (GMainLoop *loop)
{
    GhbWorkerCommand command = worker_get_command();
    switch (command)
    {
        case CMD_STOP:
            worker_stop();
            break;
        case CMD_PAUSE:
            worker_pause();
            break;
        case CMD_START:
            worker_start();
            break;
        default:
            break;
    }
    if (!worker_send_state(h_state))
    {
        worker_stop();
        g_main_loop_quit(loop);
        return G_SOURCE_REMOVE;
    }
    return G_SOURCE_CONTINUE;
}

/*
 * The alternative main function, called when HandBrake is started
 * in worker mode.
 */
int
ghb_worker_main (int argc, char *argv[])
{
    ghb_set_process_name("handbrake-work");
    g_set_prgname("handbrake-worker");
    g_autoptr(GError) error = NULL;
    g_autoptr(GMainLoop) loop = NULL;

    if (!worker_init(1))
    {
        g_critical("Initialization failed\n");
        return EXIT_FAILURE;
    }
    ghb_log("hb-worker: Waiting for job...");
    g_autofree char *json = worker_receive_json();
    if (json != NULL)
    {
        hb_add_json(h_job, json);
        hb_start(h_job);
        loop = g_main_loop_new(g_main_context_default(), FALSE);
        g_timeout_add(100, G_SOURCE_FUNC(worker_update_status), loop);
        g_timeout_add(100, G_SOURCE_FUNC(worker_socket_communicate), loop);
        g_main_loop_run(loop);
        ghb_log("hb-worker: Job finished, shutting down");
    }
    else
    {
        ghb_log("hb-worker: No JSON data received, shutting down");
    }
    worker_shutdown();
    return EXIT_SUCCESS;
}
