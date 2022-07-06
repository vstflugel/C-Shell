#include "def.h"

void cd(char *path, char *home)
{
    path = strtok(NULL, " \t");

    if(path == NULL)
    {
        path = "~";
    }

    char changePath[3*MAX_SIZE];

    if(path[0] == '~')
    {
        char new[MAX_SIZE];
        strcpy(new, home);
        strcat(new, path+1);
        strcpy(changePath, new);
    }

    else
    {
        strcpy(changePath, path);
    }
    
    if(chdir(changePath) < 0)
    {
        printf("cd: %s: No such file or directory\n", path);
    }
}