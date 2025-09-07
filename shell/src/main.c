#include "shell.h"
#include "prompt.h"
#include "input.h"
#include "parser.h"
#include "hop.h"
#include "reveal.h"
#include "log.h"
#include "file.h"
#include "pipe.h"
#include "activities.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <termios.h>

void print_cmd_groups(cmd_group *head)
{
    int group_num = 0;
    while (head)
    {
        printf("Group %d: background=%d, and=%d, semi_col=%d\n", group_num, head->background, head->and, head->semi_col);
        atomic *atom = head->atom;
        int atom_num = 0;
        while (atom)
        {
            printf("  Atom %d: cmd='%s', args=%s, redir=%d, input_file='%s', output_file='%s'\n",
                   atom_num,
                   atom->command ? atom->command : "(null)",
                   atom->args ? atom->args : "(null)",
                   atom->redirection,
                   atom->input_file ? atom->input_file : "(null)",
                   atom->output_file ? atom->output_file : "(null)");
            atom = atom->next;
            atom_num++;
        }
        head = head->next;
        group_num++;
    }
}

void sigint_handler(int signo)
{
    if (forepgrp > 0)
    {
        // printf("Sending SIGINT to foreground process %d\n", forepgrp);
        kill(forepgrp, SIGINT);
    }
    else
        return;
}

void sigtstp_handler(int signo)
{
    if (forepgrp > 0)
    {
        kill(forepgrp, SIGTSTP);

        for (int i = 0; i < job_number; i++)
        {
            if (process_list[i].state == 1 && process_list[i].background == 0)
            {
                process_list[i].state = 0;
                process_list[i].background = 1;
                printf("[%d] Stopped %s\n", process_list[i].job_number, process_list[i].command_name);
            }
        }
        fflush(stdout);
        forepgrp = -1;
    }
    else
        return;
}

// void sigchld_handler(int sig)
// {
//     int saved_errno = errno;
//     pid_t pid;
//     int status;

//     // Reap *stopped* or *terminated* children
//     while ((pid = waitpid(-1, &status, WUNTRACED | WNOHANG)) > 0)
//     {
//         if (WIFSTOPPED(status))
//         {
//             printf("Child %d stopped\n", pid);
//         }
//         else if (WIFEXITED(status) || WIFSIGNALED(status))
//         {
//             printf("Child %d exited\n", pid);
//         }
//     }

//     errno = saved_errno;
// }

int main(void)
{
    // struct sigaction sa;
    // sa.sa_handler = sigchld_handler;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;
    // sigaction(SIGCHLD, &sa, NULL);

    char *input;
    init_prompt_info();
    init_process_list();
    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGTTOU, SIG_IGN);

    while (1)
    {
        check_background_jobs();
        update_process_states();
        display_prompt();
        input = get_user_input();
        if (strlen(input) == 0)
            continue;
        check_background_jobs();
        char **parsed = NULL;
        cmd_group *groups = parse_input(input, &parsed);
        if (groups == NULL)
        {
            printf("Invalid Syntax!\n");
            continue;
        }
        cmd_group *curr = groups;
        run_command(curr);
        store_command(input);
        // print_cmd_groups(groups);
        free_cmd_groups(groups);
    }
    return 0;
}
