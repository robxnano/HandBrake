/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright (C) 2023 HandBrake Team */

#define G_LOG_DOMAIN "hb-server"

#ifdef __linux__
#define _GNU_SOURCE
#endif

#include <gio/gio.h>
#include <gio/gunixsocketaddress.h>
#include <pthread.h>

#include "server.h"
#include "application.h"

static GSocket *server_socket;
static ghb_worker_status_t *worker;
static char *job_json;

gboolean server_send_json(const char *json);

/**
 * Sets the name of a process using the appropriate pthread API.
 * This is the name that appears in process viewers such as top.
 * Must be called from the main thread. On Linux, this name can
 * be a maximum of 15 characters.
 * @name: The name to set the current process to
 */
void
ghb_set_process_name (const char *name)
{
#if defined(__FreeBSD__) || defined(__OpenBSD__)
  pthread_set_name_np(pthread_self(), name);
#elif defined(__NetBSD__)
  pthread_setname_np(pthread_self(), "%s", (void *)name);
#else
  pthread_setname_np(pthread_self(), name);
#endif
}

/**
 * Get a suitable path to create a Unix socket for local IPC.
 * Typically resides in /run/user/<user_id> on Linux, depending
 * on the value of $XDG_RUNTIME_DIR.
 * Returns: (transfer full): The path to use for the socket
 */
char *
ghb_get_socket_path (void)
{
    return g_build_filename(g_get_user_runtime_dir(), "handbrake.socket", NULL);
}

static gboolean
server_accept_connection (void)
{
    g_autoptr(GError) error = NULL;

    if (g_socket_condition_check(server_socket, G_IO_IN))
    {
        worker->socket = g_socket_accept(server_socket, NULL, &error);
        worker->state = ghb_server_get_worker_state(0, worker->state);
        if (error)
        {
            ghb_log("hb-server: Could not accept connection: %s",
                    error->message);
        }
        else
        {
            g_debug("Accepted connection from worker");
            server_send_json(job_json);
            g_free(job_json);
        }
    }

    return G_SOURCE_CONTINUE;
}

/**
 * Create a socket for communication with HandBrake worker processes.
 * Newly spawned processes connect to the socket in order to receive
 * instructions, and report the job status to the server.
 * Returns: TRUE if the socket was created successfully and can be used
 * to communicate.
 */
gboolean
ghb_server_socket_init (void)
{
    g_autoptr(GError) error = NULL;
    g_autoptr(GSocketAddress) addr;
    g_autofree char *path;
    g_autoptr(GSocket) sock;
    gboolean result;

    path = ghb_get_socket_path();
    unlink(path);
    g_debug("Opening local socket %s", path);

    addr = g_unix_socket_address_new(path);
    sock = g_socket_new(G_SOCKET_FAMILY_UNIX, G_SOCKET_TYPE_STREAM,
                        G_SOCKET_PROTOCOL_DEFAULT, NULL);
    result = g_socket_bind(sock, addr, TRUE, &error);
    if (!result)
    {
        ghb_log("hb-server: %s", error->message);
        return FALSE;
    }
    result = g_socket_listen(sock, &error);
    if (!result)
    {
        ghb_log("hb-server: %s", error->message);
        return FALSE;
    }
    g_timeout_add(100, G_SOURCE_FUNC(server_accept_connection), NULL);
    server_socket = g_steal_pointer(&sock);
    return TRUE;
}

/**
 * Shuts down the server socket and frees all memory related to it.
 * Call this when shutting down the program.
 */
void
ghb_server_socket_shutdown (void)
{
    g_return_if_fail(G_IS_SOCKET(server_socket));

    g_debug("Shutting down socket");
    g_socket_shutdown(server_socket, TRUE, TRUE, NULL);
    g_object_unref(server_socket);
    if (worker)
    {
        g_socket_shutdown(worker->socket, TRUE, TRUE, NULL);
        g_object_unref(worker->socket);
        g_free(worker->state);
        g_free(worker);
    }
    server_socket = NULL;

    g_autofree char *path = ghb_get_socket_path();
    unlink(path);
}

/**
 * Send a command to a worker process such as pause, resume or stop.
 * The PID is currently unused but will be needed in the future when
 * simultaneous encode support is implemented.
 * @worker_pid: The process to send the command to
 * @command: The GhbWorkerCommand to send
 * Returns: TRUE if the command was sent successfully
 */
static gboolean
server_send_command (pid_t worker_pid, GhbWorkerCommand command)
{
    g_autoptr(GError) error = NULL;
    ssize_t data_len = 0;
    GOutputVector command_vec[] = {
        { &COMMAND_MAGIC, sizeof(uint32_t) },
        { &data_len, sizeof(ssize_t) },
        { &command, sizeof(GhbWorkerCommand) },
    };
    if (g_socket_condition_check(worker->socket, G_IO_OUT))
    {
        g_debug("sending command %d...", command);
        g_socket_send_message(worker->socket, NULL, command_vec, 3,
                              NULL, 0, G_SOCKET_MSG_NONE, NULL, &error);
        if (error)
            ghb_log("hb-server: Could not send command: %s", error->message);
    }
    else
    {
        ghb_log("hb-server: Socket was not ready to send data");
        return FALSE;
    }
    return TRUE;
}

/**
 * Send the pause command to a worker process.
 * @worker_pid: The process to send the command to
 * Returns: TRUE if the process was paused successfully
 */
gboolean
ghb_server_pause_job (pid_t worker_pid)
{
    return server_send_command(worker_pid, CMD_PAUSE);
}

/**
 * Send the resume command to a worker process.
 * @worker_pid: The process to send the command to
 * Returns: TRUE if the process was resumed successfully
 */
gboolean
ghb_server_resume_job (pid_t worker_pid)
{
    return server_send_command(worker_pid, CMD_RESUME);
}

/**
 * Send the stop (cancel) command to a worker process.
 * @worker_pid: The process to send the command to
 * Returns: TRUE if the process was stop successfully
 */
gboolean
ghb_server_stop_job (pid_t worker_pid)
{
    return server_send_command(worker_pid, CMD_STOP);
}

gboolean
server_send_json (const char *json)
{
    g_return_val_if_fail(json != NULL, FALSE);
    g_return_val_if_fail(G_IS_SOCKET(worker->socket), FALSE);

    g_autoptr(GError) error = NULL;
    ssize_t len = strlen(json) + 1;
    GOutputVector header_vec[] = {
        { &JSON_MAGIC, sizeof(uint32_t) },
        { &len, sizeof(ssize_t) },
    };
    // Wait to ensure the worker process is ready
    if (g_socket_condition_timed_wait(worker->socket, G_IO_OUT, 100 * 1000, NULL, NULL))
    {
        g_socket_send_message(worker->socket, NULL, header_vec, 2, NULL, 0,
                              G_SOCKET_MSG_NONE, NULL, NULL);
        g_socket_send(worker->socket, json, len, NULL, &error);
        if (error)
            ghb_log("hb-server: Could not send JSON: %s", error->message);
        else
            g_debug("Sent JSON (%zd bytes)", len);
    }
    else
    {
        ghb_log("hb-server: Socket was not ready to send data");
        return FALSE;
    }
    return TRUE;
}

/**
 * Receives the latest status from the worker in the form of a
 * hb_state_t struct.
 * @worker_pid: The worker process to monitor (currently unused)
 * @state: The hb_state_t struct to store the data in
 * Returns: (transfer full): The struct where the data was stored
 */
hb_state_t *
ghb_server_get_worker_state (pid_t worker_pid, hb_state_t *state)
{
    if (!server_socket || !worker || !worker->socket)
        return NULL;

    if (!state)
        state = g_malloc0(sizeof(hb_state_t));

    g_autoptr(GError) error = NULL;
    ssize_t recv = 0;
    ssize_t data_len = -1;
    uint32_t magic = 0;
    GInputVector response[] = {
        { &magic, sizeof(uint32_t) },
        { &data_len, sizeof(ssize_t) },
        { state, sizeof(hb_state_t) },
    };
    while (g_socket_get_available_bytes(worker->socket) >= RESPONSE_LEN)
    {
        recv = g_socket_receive_message(worker->socket, NULL, response, 3, NULL,
                                        0, G_SOCKET_MSG_NONE, NULL, &error);
        if (error)
            ghb_log("hb-server: Could not get response: %s", error->message);
        else if (magic != RESPONSE_MAGIC)
            ghb_log("hb-server: Received incorrect packet header");
        else
            g_debug("Successfully got worker state (%zd bytes)", recv);
    }
    ssize_t bytes_left = g_socket_get_available_bytes(worker->socket);
    if (bytes_left > 0)
    {
        ghb_log("hb-server: Clearing unread data (%zd bytes)", bytes_left);
        char *cleanup = g_malloc0(bytes_left);
        g_socket_receive(worker->socket, cleanup, bytes_left, NULL, NULL);
        g_free(cleanup);
    }
    return state;
}

gboolean
server_monitor_worker_state (void)
{
    if (g_socket_get_available_bytes(worker->socket) >= 0)
    {
        worker->state = ghb_server_get_worker_state(0, worker->state);
    }
    return G_SOURCE_CONTINUE;
}

/**
 * Starts a new worker process ready to receive a job.
 * @job_dict: The GhbValue containing the details of the job
 * Returns: The PID of the created worker process. Use this to monitor
 * the progress of the job.
 */
pid_t
ghb_server_start_worker (const GhbValue *job_dict)
{
    gboolean result;
    GPid pid;
    g_autoptr(GError) error = NULL;
    g_autofree char *app_path = ghb_application_get_app_path(GHB_APPLICATION_DEFAULT);
    char *argv[] = { app_path, "--worker", "--verbose", NULL };

    result = g_spawn_async_with_fds(NULL, argv, NULL, G_SPAWN_DEFAULT,
                                    NULL, NULL, &pid, -1, 1, 2, &error);

    if (!result)
    {
        g_critical("Could not start worker: %s", error->message);
        return -1;
    }

    ghb_log("hb-server: Started worker with pid %d", pid);
    worker = g_malloc0(sizeof(ghb_worker_status_t));
    //g_timeout_add(200, G_SOURCE_FUNC(server_monitor_worker_state), NULL);
    job_json = hb_value_get_json(job_dict);
    worker->pid = pid;
    return pid;
}
