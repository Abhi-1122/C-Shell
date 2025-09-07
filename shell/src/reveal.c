// ########## LLM GENERATED CODE BEGINS ##########
#include "reveal.h"
#include "file.h"
#include <string.h>
#include <ctype.h>

static char previous_directory[MAX_PATH_SIZE] = "";
static int has_previous = 0;

static int compare_strings(const void *a, const void *b) {
    const unsigned char *sa = (const unsigned char *) *(const char **)a;
    const unsigned char *sb = (const unsigned char *) *(const char **)b;
    while (*sa && *sb) {
        int ca = tolower((int)*sa);
        int cb = tolower((int)*sb);
        if (ca != cb) return ca - cb;
        sa++; sb++;
    }
    if (*sa == 0 && *sb == 0) return strcmp(*(const char **)a, *(const char **)b);
    return *sa ? 1 : -1;
}

static void print_file_details(const char *filename, const char *dir_path) {
    struct stat file_stat;
    char full_path[MAX_PATH_SIZE];
    
    snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, filename);
    
    if (stat(full_path, &file_stat) == -1) {
        perror("stat");
        return;
    }
    
    // Print filename
    printf("%s\n", filename);
}

static int reveal_directory(const char *path, int show_all, int long_format) {
    DIR *dir;
    struct dirent *entry;
    char **file_list = NULL;
    int file_count = 0;
    int capacity = 10;
    
    dir = opendir(path);
    if (dir == NULL) {
        perror("reveal");
        return -1;
    }
    
    // Allocate initial memory for file list
    file_list = malloc(capacity * sizeof(char*));
    if (file_list == NULL) {
        printf("Memory allocation failed\n");
        closedir(dir);
        return -1;
    }
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip hidden files unless -a flag is set
        if (!show_all && entry->d_name[0] == '.') {
            continue;
        }
        
        // Resize array if needed
        if (file_count >= capacity) {
            capacity *= 2;
            file_list = realloc(file_list, capacity * sizeof(char*));
            if (file_list == NULL) {
                printf("Memory allocation failed\n");
                closedir(dir);
                return -1;
            }
        }
        
        // Allocate memory for filename and copy it
        file_list[file_count] = malloc(strlen(entry->d_name) + 1);
        if (file_list[file_count] == NULL) {
            printf("Memory allocation failed\n");
            closedir(dir);
            return -1;
        }
        strcpy(file_list[file_count], entry->d_name);
        file_count++;
    }
    
    closedir(dir);
    
    // Sort files lexicographically
    qsort(file_list, file_count, sizeof(char*), compare_strings);
    
    // Print files
    if (long_format) {
        // Print one file per line with details
        for (int i = 0; i < file_count; i++) {
            print_file_details(file_list[i], path);
        }
    } else {
        // Print files in ls format (space separated)
        for (int i = 0; i < file_count; i++) {
            printf("%s", file_list[i]);
            if (i < file_count - 1) {
                printf(" ");
            }
        }
        if (file_count > 0) {
            printf("\n");
        }
    }
    
    // Free allocated memory
    for (int i = 0; i < file_count; i++) {
        free(file_list[i]);
    }
    free(file_list);
    
    return 0;
}

static char* resolve_path(const char *arg) {
    static char resolved_path[MAX_PATH_SIZE];
    
    if (strcmp(arg, "~") == 0) {
        strcpy(resolved_path, home_directory);
    }
    else if (strcmp(arg, ".") == 0) {
        if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
            perror("getcwd");
            return NULL;
        }
    }
    else if (strcmp(arg, "..") == 0) {
        if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
            perror("getcwd");
            return NULL;
        }
        // Find parent directory
        char *last_slash = strrchr(resolved_path, '/');
        if (last_slash != NULL && last_slash != resolved_path) {
            *last_slash = '\0';
        } else if (last_slash == resolved_path) {
            strcpy(resolved_path, "/");
        }
    }
    else if (strcmp(arg, "-") == 0) {
        if (!has_previous) {
            if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
                perror("getcwd");
                return NULL;
            }
        } else {
            strcpy(resolved_path, previous_directory);
        }
    }
    else {
        // Handle relative or absolute path
        if (arg[0] == '/') {
            // Absolute path
            strcpy(resolved_path, arg);
        } else {
            // Relative path
            if (getcwd(resolved_path, sizeof(resolved_path)) == NULL) {
                perror("getcwd");
                return NULL;
            }
            strcat(resolved_path, "/");
            strcat(resolved_path, arg);
        }
    }
    
    return resolved_path;
}

int reveal_command(char *tokens) {
    int show_all = 0;
    int long_format = 0;
    char *path_arg = ".";
    char current_dir[MAX_PATH_SIZE];
    int len = strlen(tokens);

    // Get current directory for previous directory tracking
    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        strcpy(previous_directory, current_dir);
        has_previous = 1;
    }
    
    // Parse tokens
    for (int i = 0; i < len; i++) {
        if (tokens[i] == '-') {
            while(tokens[i]!= ' ' && tokens[i]!= '\0')
            {
                if (tokens[i] == 'a') {
                    show_all = 1;
                } else if (tokens[i] == 'l') {
                    long_format = 1;
                }
                i++;
            }
        } else {
            path_arg = strdup(tokens + i);
            break;
        }
    }
    
    // Resolve the path
    char *resolved_path = resolve_path(path_arg);
    if (resolved_path == NULL) {
        return -1;
    }
    
    // Reveal the directory
    if (reveal_directory(resolved_path, show_all, long_format) != 0) {
        return -1;
    }
    
    return 0;
}

// ########## LLM GENERATED CODE ENDS ##########