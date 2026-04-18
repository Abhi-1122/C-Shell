#ifndef ACTIVITIES_H
#define ACTIVITIES_H

#include "shell.h"
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 1024

typedef struct {
    int job_number;
    pid_t pid;
    char command_name[256];
    int state; // 0: stopped, 1: running, 2: terminated
    int background; // 1: background job, 0: foreground job
} process_info_t;

// Activities command functions
void init_process_list(void);
int activities_command(char **tokens, int token_count);
int add_process(pid_t pid, const char *command_name, int background);
void update_process_states(void);
void check_background_jobs(void);

// Global process list
extern process_info_t process_list[MAX_PROCESSES];
extern int job_number;
#endif
