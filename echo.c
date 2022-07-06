#include "def.h"

void exEcho(char *cur)
{
    cur = strtok(NULL, " \t");

    while(cur != NULL)
    {
        int n = strlen(cur);
        int doubleQuotes = 0;
        int singleQuotes = 0;

        for(int i=0; i<n; i++)
        {
            if(cur[i] == '"')
            {
                doubleQuotes++;
            }
            
            else if(cur[i] == '\'')
            {
                singleQuotes++;
            }
        }

        for(int i = 0; i < n; i++)
        {
            if(singleQuotes%2 == 0 && doubleQuotes%2 == 0)
            {
                if(cur[i] == '"' || cur[i] == '\'')
                {
                    continue;
                }

                else
                {
                    printf("%c", cur[i]);
                }
            }

            else if(singleQuotes%2 == 0)
            {
                if(cur[i] == '\'')
                {
                    continue;
                }

                else
                {
                    printf("%c", cur[i]);
                }
            }

            else if(doubleQuotes%2 == 0)
            {
                if(cur[i] == '"')
                {
                    continue;
                }

                else
                {
                    printf("%c", cur[i]);
                }
            }

            else
            {
                printf("%c", cur[i]);
            }
        }

        printf(" ");
        cur = strtok(NULL, " \t");
    }
    printf("\n");
}