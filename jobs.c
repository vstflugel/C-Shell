#include "def.h"

void jobs(process proc[])
{
    int numberOfProcesses = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(proc[i].pid != -1)
        {
            // printf("* %d *\n", i);
            numberOfProcesses++;
            printf("[%d] ", numberOfProcesses);

            char checkStatus[MAX_SIZE] = "/proc/";
            char pid[MAX_SIZE];
            sprintf(pid, "%d", proc[i].pid);
            strcat(checkStatus, pid);
            strcat(checkStatus, "/stat");

            FILE *file = fopen(checkStatus, "r");
            char f[MAX_SIZE], s[MAX_SIZE], t[MAX_SIZE] = "";
            fscanf(file, "%s %s %s", f, s, t);

            if(strcmp(t, "T") == 0)
            {
                printf("Stopped %s[%d]\n", proc[i].name, proc[i].pid);   
            }

            else
            {
                printf("Running %s[%d]\n", proc[i].name, proc[i].pid);
            }
        }
    }
}

void sig(char *token, process proc[])
// void kjob(char *token, process proc[])
{
    token = strtok(NULL, " \t");

    if(token == NULL)
    {
        perror("Invalid number of arguments");
        return;
    }

    char jobNumber[MAX_SIZE], signalNumber[MAX_SIZE];

    strcpy(jobNumber, token);

    token = strtok(NULL, " \t");

    if(token == NULL)
    {
        perror("Invalid number of arguments");
        return;
    }

    strcpy(signalNumber, token);

    token = strtok(NULL, " \t");

    if(token != NULL)
    {
        perror("Invalid number of arguments");
        return;
    }

    int job = atoi(jobNumber);
    int sig = atoi(signalNumber);

    int processesSoFar = 0;

    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(proc[i].pid != -1)
        {
            processesSoFar++;

            if(processesSoFar == job)
            {
                if(kill(proc[i].pid, sig) < 0)
                {
                    perror("Invalid signal number\n");
                }
                
                return;
            }
        }
    }

    printf("Invalid job number\n");
}

void overkill(process proc[])
{
    for(int i = 0; i < MAX_SIZE; i++)
    {
        if(proc[i].pid != -1)
        {
            kill(proc[i].pid, 9);
        }
    }
}