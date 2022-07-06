#include "def.h"

void pinfo(char *token, char *home)
{
    token = strtok(NULL, " \t");

    char pid[MAX_SIZE] = "";

    if(token == NULL)
    {
        int get = getppid();

        sprintf(pid, "%d", get);
    }

    else
    {
        int i = 0;
        for(i=0; token[i] != '\0'; i++)
        {
            pid[i] = token[i];
        }
        pid[i] = '\0';
    }

    char cur[MAX_SIZE] = "/proc/";

    strcat(cur, pid);

    char processStatus[MAX_SIZE] = "";

    strcpy(processStatus, cur);
    strcat(processStatus, "/stat");

    FILE *file = fopen(processStatus, "r");

    if(file == NULL)
    {
        printf("No process with pid: %s\n", pid);
        return;    
    }

    else
    {
        char f[MAX_SIZE], s[MAX_SIZE], t[MAX_SIZE] = "";

        fscanf(file, "%s %s %s", f, s, t);

        printf("pid -- %s\n", pid);
        printf("Process Status -- %s\n", t);
    }

    char virtualMemory[MAX_SIZE] = "";

    strcpy(virtualMemory, cur);
    strcat(virtualMemory, "/statm");

    file = fopen(virtualMemory, "r");

    char f[MAX_SIZE] = "";

    fscanf(file, "%s", f);

    printf("memory -- %s  {Virtual Memory}\n", f);

    char sendPath[MAX_SIZE] = "";
    strcpy(sendPath, cur);
    strcat(sendPath, "/exe");

    char executablePath[MAX_SIZE];

    readlink(sendPath, executablePath, MAX_SIZE);

    executablePath[strlen(executablePath)] = '\0';

    int n1 = strlen(home);
    int n2 = strlen(executablePath);

    int i = 0;
    // printf("%d %d %d %s %s %s\n", n1, n2, i, home, executablePath, sendPath);
    for(i = 0; i < n1 && i < n2; i++)
    {
        if(executablePath[i] != home[i])
        {
            break;
        }
    }

    if(i == n1)
    {
        if(n1 == n2 || (n2 > n1 && executablePath[n1] == '/'))
        {
            executablePath[0] = '~';

            for(int i=n1; i<n2; i++)
            {
                executablePath[i-n1+1] = executablePath[i];
            }
            executablePath[n2-n1+1] = '\0';
        }
    }

    printf("Executable Path -- %s\n", executablePath);
}