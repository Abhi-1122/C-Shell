#include "input.h"

char *get_user_input(void)
{
    static char input[MAX_INPUT_SIZE];

    int i = 0;
    char c;
    int ret = scanf("%c", &c);

    if (ret == -1)
    {
        printf("\nlogout");
        for (int i = 0; i < job_number; i++)
        {
            if (process_list[i].state == 1)
            {
                kill(process_list[i].pid, SIGKILL);
            }
        }
        exit(0);
    }

    while (i < MAX_INPUT_SIZE - 1 && c != '\n' && ret != -1)
    {
        input[i++] = c;
        ret = scanf("%c", &c);
    }
    input[i] = '\0';

    return input;
}
