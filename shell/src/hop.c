// ########## LLM GENERATED CODE BEGINS ##########

#include "hop.h"
#include "file.h"

char previous_directory[MAX_PATH_SIZE] = "";
int has_previous = 0;

static int change_to_directory(const char *path)
{
    char current_path[MAX_PATH_SIZE];

    if (getcwd(current_path, sizeof(current_path)) == NULL)
    {
        printf("Error getting current working directory\n");
        return -1;
    }

    if (chdir(path) != 0)
    {
        perror("hop");
        return -1;
    }

    strcpy(previous_directory, current_path);
    has_previous = 1;

    return 0;
}

// int hop_command(char *tokens)
// {
//     if (strcmp(tokens, "|") == 0 || strcmp(tokens, "&") == 0 ||
//         strcmp(tokens, "&&") == 0 || strcmp(tokens, ";") == 0 )
//     {
//         return change_to_directory(home_directory);
//     }
//     if (tokens[0] == '<' || tokens[0] == '>')
//     {
//         return -1;
//     }

//     if (strcmp(tokens, "~") == 0)
//     {
//         if (change_to_directory(home_directory) != 0)
//         {
//             return -1;
//         }
//     }
//     else if (strcmp(tokens, ".") == 0)
//     {
//         return 0;
//     }
//     else if (strcmp(tokens, "..") == 0)
//     {
//         if (change_to_directory("..") != 0)
//         {
//             return -1;
//         }
//     }
//     else if (strcmp(tokens, "-") == 0)
//     {
//         if (!has_previous)
//         {
//             return 0;
//         }

//         char temp_path[MAX_PATH_SIZE];
//         strcpy(temp_path, previous_directory);

//         if (change_to_directory(temp_path) != 0)
//         {
//             return -1;
//         }
//     }
//     else
//     {
//         if (change_to_directory(tokens) != 0)
//             return -1;
//     }

//     return 0;
// }

int hop_command(char *tokens) {
    char *arg = strtok(tokens, " ");
    while (arg != NULL) {
        if (strcmp(arg, "~") == 0) {
            if (change_to_directory(home_directory) != 0) return -1;
        } else if (strcmp(arg, ".") == 0) {
            // do nothing
        } else if (strcmp(arg, "..") == 0) {
            if (change_to_directory("..") != 0) return -1;
        } else if (strcmp(arg, "-") == 0) {
            if (has_previous) {
                if (change_to_directory(previous_directory) != 0) return -1;
            }
        } else {
            if (change_to_directory(arg) != 0) return -1;
        }
        arg = strtok(NULL, " ");
    }
    return 0;
}


// ########## LLM GENERATED CODE ENDS ##########
