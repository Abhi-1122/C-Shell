#ifndef PIPE_H
#define PIPE_H

#include "shell.h"
#include "parser.h"
#include "file.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "activities.h"
#include "intrinsic.h"
#include <sys/wait.h>
#include <termios.h>

#define MAX_PIPES 100

int run_pipeline(atomic *atom_head, int background);
void run_command(cmd_group *curr);

#endif
