#ifndef LOG_H
#define LOG_H

#include "shell.h"

#define MAX_LOG_ENTRIES 15
#define LOG_FILENAME "shell_log.txt"

// Log command functions
int log_command(char **tokens, int token_count);
int log_command_with_redirection(char **tokens, int token_count);
void store_command(const char *command);
void load_log_entries(char log_entries[][MAX_INPUT_SIZE], int *count);
void save_log_entries(char log_entries[][MAX_INPUT_SIZE], int count);
int execute_log_entry(int index);

#endif
