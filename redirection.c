#include "def.h"

int checkOutputRedirection(char curCommand[], int *outputFileLength, char outputFile[])
{
    for(int i = 0; curCommand[i] != '\0'; i++)
    {
        if(curCommand[i] == '>')
        {
            if(curCommand[i+1] == '>')
            {
                i+=2;
                while(curCommand[i] != '\0' && curCommand[i] == ' ')
                {
                    i++;
                }

                int j = i;
                for(j = i; curCommand[j] != '\0' &&  curCommand[j] != '<' && curCommand[j] != ' ' && curCommand[j] != '|'; j++)
                {
                    outputFile[j-i] = curCommand[j];
                    (*outputFileLength)++;
                }
                outputFile[j-i] = '\0';
                return 2;
            }

            i++;
            while(curCommand[i] != '\0' && curCommand[i] == ' ')
            {
                i++;
            }

            int j = i;
            for(j = i; curCommand[j] != '\0' && curCommand[j] != '<' && curCommand[j] != ' ' && curCommand[j] != '|'; j++)
            {
                outputFile[j-i] = curCommand[j];
                (*outputFileLength)++;
            }
            outputFile[j-i] = '\0';
            return 1;
        }
    }

    return 0;
}

int checkInputRedirection(char curCommand[], int *inputFileLength, char inputFile[])
{
    for(int i = 0; curCommand[i] != '\0'; i++)
    {
        if(curCommand[i] == '<')
        {
            i++;
            while(curCommand[i] != '\0' && curCommand[i] == ' ')
            {
                i++;
            }

            int j = i;
            for(j = i; curCommand[j] != '\0' &&  curCommand[j] != '>' && curCommand[j] != ' ' && curCommand[j] != '|'; j++)
            {
                inputFile[j-i] = curCommand[j];
                (*inputFileLength)++;
            }
            inputFile[j-i] = '\0';
            return 1;
        }
    }

    return 0;
}