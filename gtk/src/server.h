/* SPDX-License-Identifier: GPL-2.0-or-later */
/* Copyright (C) 2023 HandBrake Team */

#ifndef _SERVER_H_
#define _SERVER_H_

#include "callbacks.h"
#include "hb-backend.h"

typedef enum {
    CMD_NONE,
    CMD_START,
    CMD_PAUSE,
    CMD_RESUME,
    CMD_STOP,
} GhbWorkerCommand;

typedef enum {
    RSP_NONE,
    RSP_NOTSTARTED,
    RSP_RUNNING,
    RSP_PAUSED,
    RSP_COMPLETE,
    RSP_FAILED,
    RSP_CANCELED,
} GhbWorkerResponse;

typedef struct {
    pid_t pid;
    GSocket *socket;
    hb_state_t *state;
    ghb_instance_status_t status;
} ghb_worker_status_t;

static const int32_t COMMAND_MAGIC = 0x48423143L;  // HB1C
static const int32_t JSON_MAGIC = 0x4842314AL;     // HB1J
static const int32_t RESPONSE_MAGIC = 0x48423152L; // HB1R

static const ssize_t HEADER_LEN = sizeof(uint32_t) + sizeof(ssize_t);
static const ssize_t RESPONSE_LEN = HEADER_LEN + sizeof(hb_state_t);
static const ssize_t COMMAND_LEN = HEADER_LEN + sizeof(GhbWorkerCommand);
static const ssize_t JSON_LEN = HEADER_LEN + sizeof(ssize_t);

int ghb_worker_main(int argc, char *argv[]);

void ghb_set_process_name(const char *name);

gboolean ghb_server_socket_init(void);
void ghb_server_socket_shutdown(void);
pid_t ghb_server_start_worker(const GhbValue *job_dict);
char *ghb_get_socket_path(void);

gboolean ghb_server_pause_job(pid_t worker_pid);
gboolean ghb_server_resume_job(pid_t worker_pid);
gboolean ghb_server_stop_job(pid_t worker_pid);
hb_state_t *ghb_server_get_worker_state(pid_t worker_pid, hb_state_t *state);
#endif // _SERVER_H_
