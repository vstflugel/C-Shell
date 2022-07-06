#include "def.h"

void bg(char *token, process proc[])
{
    token = strtok(NULL, " \t");

    if(token == NULL)
    {
        perror("Invalid number of arguments");
        return;
    }

    char jobNumber[MAX_SIZE];
    strcpy(jobNumber, token);

    token = strtok(NULL, " \t");

    if(token != NULL)
    {
        perror("Invalid number of arguments");
        return;
    }

    int job = atoi(jobNumber);

    int processesSoFar = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(proc[i].pid != -1)
        {
            processesSoFar++;

            if(processesSoFar == job)
            {
                if(kill(proc[i].pid, SIGCONT) < 0)
                {
                    perror("Something went wrong. Please try again later.\n");
                }
                
                return;
            }
        }
    }

    printf("Invalid job number\n");
}