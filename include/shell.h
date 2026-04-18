#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <limits.h>

#define MAX_INPUT_SIZE 4096
#define MAX_PATH_SIZE 4096

// Function declarations
void display_prompt(void);
char* get_user_input(void);
void init_prompt_info(void);
char* get_current_path_display(void);

// Global variables
extern char home_directory[MAX_PATH_SIZE];
extern char username[256];
extern char hostname[256];
extern int forepgrp;
extern int bgpgrp;
extern int shell_pgid;

#endif
