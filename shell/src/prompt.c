#include "prompt.h"
#include <stdio.h>
// ########## LLM GENERATED CODE BEGINS ##########
#define PATH_MAX 4096

char home_directory[PATH_MAX];
char username[256];
char hostname[256];

void init_prompt_info(void) {
    // Get username
    struct passwd *pw = getpwuid(getuid());
    if (pw != NULL) {
        strcpy(username, pw->pw_name);
    } else {
        strcpy(username, "unknown");
    }
    
    // Get hostname
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "unknown");
    }
    
    // Get and store home directory (current working directory at start)
    if (getcwd(home_directory, PATH_MAX) == NULL) {
        strcpy(home_directory, "/");
    }
}

char* get_current_path_display(void) {
    static char display_path[PATH_MAX];
    char current_path[PATH_MAX];
    
    if (getcwd(current_path, sizeof(current_path)) == NULL) {
        printf("Error getting current working directory\n");
        strcpy(display_path, "/");
        return display_path;
    }
    // printf("cwd: %s\n", current_path);
    // Check if current path starts with home directory
    size_t home_len = strlen(home_directory);
    if (strncmp(current_path, home_directory, home_len) == 0) {
        if (current_path[home_len] == '\0') {
            // We are exactly in home directory
            strcpy(display_path, "~");
        } else if (current_path[home_len] == '/') {
            // We are in a subdirectory of home
            strcpy(display_path, "~");
            strcat(display_path, current_path + home_len);
        } else {
            // Home is not an ancestor, display full path
            strcpy(display_path, current_path);
        }
    } else {
        // Home is not an ancestor, display full path
        strcpy(display_path, current_path);
    }
    
    return display_path;
}

void display_prompt(void) {
    char* current_path = get_current_path_display();
    printf("<%s@%s:%s> ", username, hostname, current_path);
    fflush(stdout);
}

// ########## LLM GENERATED CODE ENDS ##########