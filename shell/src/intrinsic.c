#include "intrinsic.h"

int ping(char *tokens)
{
    char *argv[15];
    int argc = 0;
    char *token = strtok(tokens, " ");
    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }

    if (argc != 2)
    {
        printf("Invalid number of arguments\n");
        return -1;
    }

    pid_t pid = (pid_t)atoi(argv[0]);
    int signal_number = atoi(argv[1]);
    int actual_signal = signal_number % 32;

    if (kill(pid, 0) == -1)
    {
        if (errno == ESRCH)
        {
            printf("No such process found\n");
        }
        else if (errno == EPERM)
        {
            printf("Process exists but no permission to send signal\n");
        }
        else
        {
            perror("kill");
        }
        return -1;
    }

    if (kill(pid, actual_signal) == -1)
    {
        perror("kill");
        return -1;
    }

    printf("Sent signal %d to process with pid %d\n", signal_number, pid);
    return 0;
}

int fg(char *tokens)
{
    pid_t target_pid = -1;
    int target_index = -1;
    int job_id = 0;
    if (tokens)
    {
        char *argv[15];
        int argc = 0;
        char *token = strtok(tokens, " ");
        while (token != NULL)
        {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        if (argc != 1)
        {
            printf("Invalid number of arguments\n");
            return -1;
        }

        job_id = atoi(argv[0]);

        for (int i = 0; i < job_number; i++)
        {
            if (process_list[i].job_number == job_id && process_list[i].background == 1)
            {
                target_pid = process_list[i].pid;
                target_index = i;
                break;
            }
        }
    }
    else
    {
        for (int i = job_number - 1; i >= 0; i--)
        {
            if (process_list[i].background == 1 || process_list[i].state == 0)
            {
                target_pid = process_list[i].pid;
                target_index = i;
                job_id = process_list[i].job_number;
                break;
            }
        }
    }

    if (target_pid == -1)
    {
        printf("No such job found\n");
        return -1;
    }

    if (kill(target_pid, 0) == -1)
    {
        printf("Process no longer exists\n");
        return -1;
    }

    if (process_list[target_index].state == 0)
    {
        if (kill(target_pid, SIGCONT) == -1)
        {
            perror("Failed to continue process");
            return -1;
        }
    }
    printf("[%d] %s\n", process_list[target_index].job_number, process_list[target_index].command_name);

    forepgrp = target_pid;
    setpgid(target_pid, target_pid);
    tcsetpgrp(STDIN_FILENO, target_pid);

    process_list[target_index].state = 1;     
    process_list[target_index].background = 0;

    int status;
    waitpid(target_pid, &status, WUNTRACED);

    if (WIFSTOPPED(status))
    {
        process_list[target_index].state = 0;
        process_list[target_index].background = 1;
        printf("[%d] Stopped %s\n", job_id, process_list[target_index].command_name);
    }

    tcsetpgrp(STDIN_FILENO, getpgrp());
    forepgrp = -1;

    return 0;
}

int bg(char *tokens)
{
    pid_t target_pid = -1;
    int target_index = -1;
    int job_id = 0;
    if (tokens)
    {
        char *argv[15];
        int argc = 0;
        char *token = strtok(tokens, " ");
        while (token != NULL)
        {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }

        if (argc != 1)
        {
            printf("Invalid number of arguments\n");
            return -1;
        }

        job_id = atoi(argv[0]);

        for (int i = 0; i < job_number; i++)
        {
            if (process_list[i].job_number == job_id && process_list[i].background == 1)
            {
                target_pid = process_list[i].pid;
                target_index = i;
                break;
            }
        }
    }
    else
    {
        for (int i = job_number - 1; i >= 0; i--)
        {
            if (process_list[i].background == 1 && process_list[i].state == 0)
            {
                target_pid = process_list[i].pid;
                target_index = i;
                job_id = process_list[i].job_number;
                break;
            }
        }
    }

    if (target_pid == -1)
    {
        printf("No such job found\n");
        return -1;
    }


    if (kill(target_pid, 0) == -1)
    {
        printf("Process no longer exists\n");
        return -1;
    }


    if (process_list[target_index].state == 0)
    {
        if (kill(target_pid, SIGCONT) == -1)
        {
            perror("Failed to continue process");
            return -1;
        }
    }



    process_list[target_index].state = 1;

    printf("[%d] %s &\n", process_list[target_index].job_number, process_list[target_index].command_name);
    return 0;
}
