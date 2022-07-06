#include "def.h"

void prompt(char *home)
{
    char s[MAX_SIZE] = "", *t, curPath[MAX_SIZE] = "";

    t = getenv("LOGNAME");
    gethostname(s, MAX_SIZE);

    getcwd(curPath, MAX_SIZE);

    // Checking if replacable by '~'
    int n1 = strlen(home);
    int n2 = strlen(curPath);

    int i = 0;
    for(i = 0; i < n1 && i < n2; i++)
    {
        if(curPath[i] != home[i])
        {
            break;
        }
    }

    // printf("%s %s %d %d\n", curPath, home, n1, n2);

    if(i == n1)
    {
        if(n1 == n2 || (n2 > n1 && curPath[n1] == '/'))
        {
            curPath[0] = '~';

            for(int i=n1; i<n2; i++)
            {
                // printf("%c ", curPath[i]);
                curPath[i-n1+1] = curPath[i];
            }
            curPath[n2-n1+1] = '\0';
        }
    }

    printf("<%s@%s:" "%s> " RESET, t, s, curPath);

    return;
}