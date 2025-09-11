// ########## LLM GENERATED CODE BEGINS ##########
#include "reveal.h"
#include "file.h"
#include <string.h>
#include <ctype.h>

static int compare_strings(const void *a, const void *b) {
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
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
    char *path_arg = NULL;
    int show_previous = 0;

    // Tokenize input by spaces
    char *tokens_copy = strdup(tokens);
    char *tok = strtok(tokens_copy, " ");
    
    while (tok) {
        if (tok[0] == '-' && strlen(tok) > 1) {
            // Parse flags like -a -l
            for (int j = 1; tok[j]; j++) {
                if (tok[j] == 'a') show_all = 1;
                else if (tok[j] == 'l') long_format = 1;
                else {
                    printf("reveal: Invalid Syntax!\n");
                    free(tokens_copy);
                    return -1;
                }
            }
        } else if (strcmp(tok, "-") == 0) {
            // Special case: reveal -
            show_previous = 1;
        } else {
            if (path_arg != NULL) {
                // Already had one path -> multiple paths detected
                printf("reveal: Invalid Syntax!\n");
                free(tokens_copy);
                return -1;
            }
            path_arg = tok;
        }
        tok = strtok(NULL, " ");
    }

    if (show_previous) {
        if (!has_previous) {
            printf("reveal: No such directory!\n");
            free(tokens_copy);
            return -1;
        }
        path_arg = previous_directory;
    }

    if (!path_arg) path_arg = "."; // default path

    // Resolve the path
    char *resolved_path = resolve_path(path_arg);
    if (!resolved_path) {
        free(tokens_copy);
        return -1;
    }

    // Reveal directory
    int res = reveal_directory(resolved_path, show_all, long_format);
    free(tokens_copy);
    return res;
}


// ########## LLM GENERATED CODE ENDS ##########