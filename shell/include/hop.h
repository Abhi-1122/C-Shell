#ifndef HOP_H
#define HOP_H

#include "shell.h"
#include <string.h>

extern char previous_directory[MAX_PATH_SIZE];
extern int has_previous;

int hop_command(char *tokens);

#endif
