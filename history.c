#include "def.h"

int storeHistory(int done, char data[20][MAX_SIZE], char command[])
{
    if(strcmp(data[19], command) == 0)
    {
        return done;
    }
    
    if(done >= 20)
    {
        for(int i = 0; i < 19; i++)
        {
            strcpy(data[i], data[i+1]);
        }

        strcpy(data[19], command);
    }

    else
    {
        for(int i = 20-done-1; i < 19; i++)
        {
            strcpy(data[i], data[i+1]);
        }

        strcpy(data[19], command);
    }

    return done+1;
}

void history(char *token, char data[20][MAX_SIZE], int done)
{
    int n = 0;

    token = strtok(NULL, " \t\n");

    if(token != NULL)
    {
        int mul = 1;
        int len = strlen(token);
        
        for(int i = len - 1; i >= 0; i--)
        {
            n += (mul*(token[i] - 48));
            mul *= 10;
        }
    }

    else
    {
        n = 10;
    }

    // printf("n = %d\n", n);

    if(n > 20)
    {
        n = 20;
    }

    if(done < n)
    {
        n = done;
    }

    for(int j = 20 - n; j < 20; j++)
    {
        printf("%s\n", data[j]);
    }
}