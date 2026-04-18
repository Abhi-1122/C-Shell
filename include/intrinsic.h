#ifndef INTRINSIC_H
#define INTRINSIC_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>
#include "activities.h"
#include "shell.h"

int ping(char* tokens);
int fg(char* tokens);
int bg(char* tokens);

#endif
