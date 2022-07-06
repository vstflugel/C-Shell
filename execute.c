#include "def.h"

void execute(char *cur, char home[], process proc[], int *proc_size, char data[20][MAX_SIZE], int done, char duplicate3[], int flag, int outputRedirection, int inputRedirection)
{
    // printf("*****\n");
    if(strcmp(cur, "quit") == 0)
    {
        exit(0);
    }

    else if(strcmp(cur, "cd") == 0)
    {
        // printf("*****\n");
        cd(cur, home);
    }

    else if(strcmp(cur, "sig") == 0)
    {
        sig(cur, proc);
    }

    else if(strcmp(cur, "overkill") == 0)
    {
        overkill(proc);
    }

    else if(strcmp(cur, "bg") == 0)
    {
        bg(cur, proc);
    }

    else if(strcmp(cur, "fg") == 0)
    {
        fg(cur, proc, &proc_size);
    }

    else if(strcmp(cur, "pwd") == 0)
    {
        pwd();
    }

    else if(strcmp(cur, "echo") == 0)
    {
        exEcho(cur);
    }

    else if(strcmp(cur, "ls") == 0)
    {
        ls(cur, home);
    }

    else if(strcmp(cur, "pinfo") == 0)
    {
        pinfo(cur, home);
    }

    else if(strcmp(cur, "history") == 0)
    {
        history(cur, data, done);
    }

  

    else if(strcmp(cur, "jobs") == 0)
    {
        // printf("*** %s\n", cur);
        jobs(proc);
    }

  

    else
    {
        if(outputRedirection > 0 || inputRedirection > 0)
        {
            strcpy(duplicate3, cur);
        }   
        // printf("%s * %s\n", cur, duplicate3);
        fg_bg(duplicate3, flag, &proc_size, proc);
    }
}
