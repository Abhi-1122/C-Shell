// ########## LLM GENERATED CODE BEGINS ##########
#include "log.h"
#include "parser.h"
#include "hop.h"
#include "reveal.h"
#include "pipe.h"
#include <string.h>
#include "file.h"

void load_log_entries(char log_entries[][MAX_INPUT_SIZE], int *count) {
    char log_path[MAX_PATH_SIZE];
    int path_len = strlen(home_directory) + strlen(LOG_FILENAME) + 2;
    
    if (path_len > MAX_PATH_SIZE) {
        *count = 0;
        return;
    }
    
    strcpy(log_path, home_directory);
    strcat(log_path, "/");
    strcat(log_path, LOG_FILENAME);
    
    int fd = open_file_for_read(log_path);
    if (fd == -1) {
        *count = 0;
        return;
    }
    
    char buffer[MAX_INPUT_SIZE * MAX_LOG_ENTRIES];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    close_file(fd);
    
    if (bytes_read <= 0) {
        *count = 0;
        return;
    }
    
    buffer[bytes_read] = '\0';
    *count = 0;
    
    char *line = strtok(buffer, "\n");
    while (line != NULL && *count < MAX_LOG_ENTRIES) {
        strcpy(log_entries[*count], line);
        (*count)++;
        line = strtok(NULL, "\n");
    }
}

void save_log_entries(char log_entries[][MAX_INPUT_SIZE], int count) {
    char log_path[MAX_PATH_SIZE];
    int path_len = strlen(home_directory) + strlen(LOG_FILENAME) + 2;
    
    if (path_len > MAX_PATH_SIZE) {
        perror("Path too long for log file");
        return;
    }
    
    strcpy(log_path, home_directory);
    strcat(log_path, "/");
    strcat(log_path, LOG_FILENAME);
    
    int fd = open_file_for_write(log_path);
    if (fd == -1) {
        perror("Failed to save log");
        return;
    }
    
    for (int i = 0; i < count; i++) {
        write(fd, log_entries[i], strlen(log_entries[i]));
        write(fd, "\n", 1);
    }
    
    close_file(fd);
}

void store_command(const char *command) {
    // Check if command contains "log" as a command name
    if (strstr(command, "log") == command || strstr(command, " log ") != NULL || 
        strstr(command, ";log") != NULL || strstr(command, "&&log") != NULL) {
        return; // Don't store log commands
    }
    
    char log_entries[MAX_LOG_ENTRIES][MAX_INPUT_SIZE];
    int count;
    
    load_log_entries(log_entries, &count);
    
    // Check if identical to last command
    if (count > 0 && strcmp(log_entries[count - 1], command) == 0) {
        return; // Don't store identical command
    }
    
    // Add new command
    if (count >= MAX_LOG_ENTRIES) {
        // Shift entries to make room
        for (int i = 0; i < MAX_LOG_ENTRIES - 1; i++) {
            strcpy(log_entries[i], log_entries[i + 1]);
        }
        count = MAX_LOG_ENTRIES - 1;
    }
    
    strcpy(log_entries[count], command);
    count++;
    
    save_log_entries(log_entries, count);
}

int execute_log_entry(int index) {
    char log_entries[MAX_LOG_ENTRIES][MAX_INPUT_SIZE];
    int count;
    
    load_log_entries(log_entries, &count);
    
    if (index < 1 || index > count) {
        printf("Error: Invalid index\n");
        return -1;
    }
    
    // Index is 1-based, newest to oldest
    int actual_index = count - index;
    char *command = log_entries[actual_index];
    
    printf("Executing: %s\n", command);
    
    // Parse and execute the command
    char **tokens = NULL;
    cmd_group *groups = parse_input(command, &tokens);
    if (groups == NULL) {
        printf("Error: Invalid syntax in stored command\n");
        return -1;
    }

    run_command(groups);
    free_cmd_groups(groups);
    return 0;
}

int log_command(char **tokens, int token_count) {
    if (token_count == 0) {
        // No arguments: print stored commands
        char log_entries[MAX_LOG_ENTRIES][MAX_INPUT_SIZE];
        int count;
        
        load_log_entries(log_entries, &count);
        
        for (int i = 0; i < count; i++) {
            printf("%s\n", log_entries[i]);
        }
        return 0;
    }
    
    if (token_count >= 1 && strcmp(tokens[0], "purge") == 0) {
        // Purge: clear the history
        char log_path[MAX_PATH_SIZE];
        int path_len = strlen(home_directory) + strlen(LOG_FILENAME) + 2;
        
        if (path_len > MAX_PATH_SIZE) {
            printf("Error: Path too long\n");
            return -1;
        }
        
        strcpy(log_path, home_directory);
        strcat(log_path, "/");
        strcat(log_path, LOG_FILENAME);
        
        int fd = open_file_for_write(log_path);
        if (fd != -1) {
            close_file(fd);
        }
        return 1;
    }
    
    if (token_count >= 2 && strcmp(tokens[0], "execute") == 0) {
        // Execute: run command at given index
        int index = atoi(tokens[1]);
        if (execute_log_entry(index) == 0) {
            return 2;
        }
        return -1;
    }
    
    printf("Error: Invalid log command syntax\n");
    return -1;
}

// ########## LLM GENERATED CODE ENDS ##########
