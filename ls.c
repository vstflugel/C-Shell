#include "def.h"

void lsna(char *actualPath, int hidden)
{
    // printf("actualPath: %s\n", actualPath);
    // printf("here\n");
    DIR *dir;
    struct dirent *ent;
    struct stat sb;

    dir = opendir(actualPath);

    if(!dir)
    {
        perror(actualPath);
        return;
    }

    char temp[MAX_SIZE];

    while((ent = readdir(dir)) != NULL)
    {
        stat(temp, &sb);

        char *curName = ent->d_name;

        if(hidden == 1 || (hidden == 0 && curName[0] != '.'))
        {
            printf("%s\n", curName);
        }
    }
    // printf("\n");

    closedir(dir);
}

void lsla(char *actualPath, int hidden)
{
    DIR *dir;
    struct dirent *ent;
    struct stat sb;
    struct tm *t;
    char date[MAX_SIZE];

    // printf("actualPath: %s\n", actualPath);

    dir = opendir(actualPath);
    if(!dir)
    {
        perror(actualPath);
        return;
    }

    long long int total = 0;
    while((ent = readdir(dir)) != NULL)
    {
        char *curName = ent->d_name;
        
        char curPath[MAX_SIZE];

        strcpy(curPath, actualPath);
        strcat(curPath, "/");
        strcat(curPath, curName);

        stat(curPath, &sb);

        if(hidden == 1 || (hidden == 0 && curName[0] != '.'))
        {
            total += sb.st_blocks;
        }
    }
    closedir(dir);
    printf("total %lld\n", total/2);


    dir = opendir(actualPath);

    while((ent = readdir(dir)) != NULL)
    {
        char *curName = ent->d_name;
        
        char curPath[MAX_SIZE];

        strcpy(curPath, actualPath);
        strcat(curPath, "/");
        strcat(curPath, curName);

        stat(curPath, &sb);


        if(hidden == 1 || (hidden == 0 && curName[0] != '.'))
        {
            // total+=sb.st_blocks;
            
            char fileType = '-';

            if(S_ISDIR(sb.st_mode))
            {
                fileType = 'd';
            }

            else if(S_ISCHR(sb.st_mode))
            {
                fileType = 'c';
            }

            else if(S_ISBLK(sb.st_mode))
            {
                fileType = 'b';
            }

            else if(S_ISREG(sb.st_mode))
            {
                fileType = '-';
            }

            else if(S_ISLNK(sb.st_mode))
            {
                fileType = 'l';
            }

            else if(S_ISSOCK(sb.st_mode))
            {
                fileType = 's';
            }

            else if(S_ISFIFO(sb.st_mode))
            {
                fileType = 'f';
            }

            printf("%c", fileType);

            printf((sb.st_mode & S_IRUSR) ? "r" : "-");
            printf((sb.st_mode & S_IWUSR) ? "w" : "-");
            printf((sb.st_mode & S_IXUSR) ? "x" : "-");
            printf((sb.st_mode & S_IRGRP) ? "r" : "-");
            printf((sb.st_mode & S_IWGRP) ? "w" : "-");
            printf((sb.st_mode & S_IXGRP) ? "x" : "-");
            printf((sb.st_mode & S_IROTH) ? "r" : "-");
            printf((sb.st_mode & S_IWOTH) ? "w" : "-");
            printf((sb.st_mode & S_IXOTH) ? "x" : "-");
            printf(" %3d", (int)sb.st_nlink);
            printf(" %8.8s", getpwuid(sb.st_uid)->pw_name);
            printf("%8.8s", getgrgid(sb.st_gid)->gr_name);
            printf(" %12lld", (long long int)sb.st_size);

            t = localtime(&sb.st_mtime);
            strftime(date, MAX_SIZE, nl_langinfo(D_T_FMT), t);
            
            for(int i=4; i<16; i++)
            {
                date[i-4] = date[i];
            }
            date[12] = '\0';

            printf(" %s %s\n", date, curName);
        }
    }

    // printf("\ntotal %d\n", total/2);

    closedir(dir);
}

void ls(char *token, char *home)
{
    char commands[3][MAX_SIZE];

    char *temp = "";

    for(int j=0; j<3; j++)
    {
        temp = strtok(NULL, " \t");

        if(temp == NULL)
        {
            for(int k = j; k < 3; k++)
            {
                strcpy(commands[k], "\0");
            }

            break;
        }

        int i = 0;
        for(i = 0; temp[i] != '\0'; i++)
        {
            commands[j][i] = temp[i];
        }
        commands[j][i] = '\0';
    }

    // printf("here\n");
    char path[MAX_SIZE];
    path[0] = '.';
    path[1] = '\0';
    int l_flag = 0, a_flag = 0;

    for(int i = 0; i < 3; i++)
    {
        // printf("here\n");
        // printf("%s\n", commands[i]);
        if(commands[i][0] == '-')
        {
            for(int j=0; commands[i][j] != '\0'; j++)
            {
                if(commands[i][j] == 'l')
                {
                    l_flag = 1;
                }

                else if(commands[i][j] == 'a')
                {
                    a_flag = 1;
                }
            }
        }

        else if(strcmp(commands[i], "\0") != 0)
        {
            strcpy(path, commands[i]);
        }
    }

    // printf("path: %s\n", path);

    char actualPath[MAX_SIZE];

    if(path[0] == '~')
    {
        strcpy(actualPath, home);
        strcat(actualPath, path + 1);
    }

    else
    {
        strcpy(actualPath, path);
    }

    if(l_flag && a_flag)
    {
        lsla(actualPath, 1);
    }

    else if(l_flag)
    {
        lsla(actualPath, 0);
    }

    else if(a_flag)
    {
        lsna(actualPath, 1);
    }

    else
    {
        lsna(actualPath, 0);
    }

}