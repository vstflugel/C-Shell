#include "def.h"

void pwd()
{
    char curPath[MAX_SIZE];
    getcwd(curPath, MAX_SIZE);

    printf("%s\n", curPath);
}