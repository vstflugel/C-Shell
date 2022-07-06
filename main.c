#include "def.h"

void handleSignal(int signal)
{
    // do nothing

    return;
}

int outputFileLength = 0;
char outputFile[MAX_SIZE];

int inputFileLength = 0;
char inputFile[MAX_SIZE];

int main(int argc, char *argv[])
{
    clearScreen();
    // printf(BLUE "Welcome to " CYAN "lsh" BLUE " aka laaaad's Shell\n\n");
    char curPath[MAX_SIZE];
    getcwd(curPath, MAX_SIZE);

    int n = strlen(argv[0]);

    for(int i = n-1; i >= 0; i--)
    {
        if(argv[0][i] == '/')
        {
            argv[0][i] = '\0';
            break;
        }
    }

    chdir(argv[0]);

    char home[MAX_SIZE];
    getcwd(home, MAX_SIZE);

    // printf("%s\n", curPath);
    chdir(curPath);

    int proc_size = 0;
    process proc[MAX_SIZE];

    for(int i = 0; i < MAX_SIZE; i++)
    {
        proc[i].pid = -1;
    }

    // For history commmand
    FILE *fp;

    char data[20][MAX_SIZE];

    int done = 0;

    char newPath[MAX_SIZE];
    strcpy(newPath, home);
    strcat(newPath, "/");
    strcat(newPath, "data.txt");
    fp = fopen(newPath, "rb");
    
    if(fp == NULL)
    {
        for(int i = 0; i < 20; i++)
        {
            strcpy(data[i], "\0");
        }
    }

    else
    {
        fread(&data, sizeof(data), 1, fp);

        for(int i = 0; i < 20; i++)
        {
            // printf("%s\n", data[i]);
            if(strcmp(data[i], "\0"))
            {
                done++;
            }
        }
        
        fclose(fp);
    }

    int stdin_fd = dup(0);
    int stdout_fd = dup(1);

    while(1)
    {
        prompt(home);

        // Disabling Ctrl + C and Ctrl + Z signals.
        signal(SIGINT, handleSignal);
        signal(SIGTSTP, handleSignal);
        signal(SIGCHLD, handleSignal);

        // For background process status printing
        int status;
        int get = waitpid(-1, &status, WNOHANG);

        if(get > 0)
        {
            if(!WIFEXITED(status))
            {
                char print[MAX_SIZE] = "";
                for(int i = 0; i < MAX_SIZE; i++)
                {
                    if(proc[i].pid == get)
                    {
                        proc[i].pid = -1;

                        for(int j = 0; proc[i].name[j] != '\0'; j++)
                        {
                            if(proc[i].name[j] == ' ')
                            {
                                print[j] = '\0';
                                break;
                            }

                            print[j] = proc[i].name[j];
                        }

                        // strcpy(print, proc[i].name);
                    }
                }

                if(strcmp(print, "") == 0)
                {
                    strcpy(print, "Process");
                }

                char exitPrint[MAX_SIZE];
                sprintf(exitPrint, "%s with pid %d exited with exit status: %d\n", print, get, WEXITSTATUS(status));
                write(2, exitPrint, strlen(exitPrint));
            }
        }

        for(int i = 0; i < MAX_SIZE; i++)
        {
            if(kill(proc[i].pid, 0) == -1)
            {
                char print[MAX_SIZE] = "";
                for(int j = 0; proc[i].name[j] != '\0'; j++)
                {
                    if(proc[i].name[j] == ' ')
                    {
                        print[j] = '\0';
                        break;
                    }

                    print[j] = proc[i].name[j];
                }

                if(strcmp(print, "") == 0)
                {
                    strcpy(print, "Process");
                }
                
                // strcpy(print, proc[i].name);
                
                char exitPrint[2*MAX_SIZE];
                sprintf(exitPrint, "%s with pid %d exited normally\n", print, proc[i].pid);
                write(2, exitPrint, strlen(exitPrint));
                // fflush(stdout);

                // prompt(home);
                proc[i].pid = -1;
            }
        }

        char *s = NULL;
        size_t len = 0;
        getline(&s, &len, stdin);

        s[strlen(s)-1] = '\0';
        
       

        char command[MAX_SIZE][MAX_SIZE];

        int numberOfCommands = 0;

        char *token = strtok(s, ";");

        while(token != NULL)
        {   
            strcpy(strcat(command[numberOfCommands++], "\0"), token);
            // printf("%s\n", strcat(token, "\0"));
            token = strtok(NULL, ";");
        }   

        int curin_fd = dup(0);
        int curout_fd = dup(1);
        
        for(int i = 0; i < numberOfCommands; i++)
        {
            // Checking if '&' is present or not
            

            // To ensure that null string does not get added in history command
            if(strcmp(command[i], "\0") == 0 || strcmp(command[i], "&") == 0)
            {
                continue;
            }

            // printf("%s cur\n", command[i]);
            char duplicate[MAX_SIZE], duplicate2[MAX_SIZE];
            strcpy(duplicate, command[i]);
            strcpy(duplicate2, command[i]);


            char *next = strtok(command[i], " \t\n");

            if(next == NULL)
            {
                continue;
            }

            if(duplicate[0] != ' ')
            {
                done = storeHistory(done, data, duplicate);
            }
            // printf("%d done\n", done);

            // for(int i = 0; i < 20; i++)
            // {
            //     printf("%s\n", data[i]);
            // }

            if(strcmp(next, "^D") == 0)
            {
             
                return 0;
            }

            else
            {
                outputFileLength = 0, inputFileLength = 0;
                int outputRedirection = checkOutputRedirection(duplicate, &outputFileLength, outputFile);
                int inputRedirection = checkInputRedirection(duplicate, &inputFileLength, inputFile);
                // printf("%s %d %d\n", outputFile, outputFileLength, outputRedirection);
                // printf("%s %d %d\n", inputFile, inputFileLength, inputRedirection);
                
                int emptyPipe = 1;

                for(int k = 0; duplicate[k] != '\0'; k++)
                {
                    if(emptyPipe == 1 && duplicate[k] == '|')
                    {
                        break;
                    }

                    // printf("%c\n", duplicate[k]);
                    if(duplicate[k] == '|')
                    {
                        emptyPipe = 1;
                    }

                    else if(duplicate[k] != ' ' && duplicate[k] != '|')
                    {
                        // printf(" * %c\n", duplicate[k]);
                        emptyPipe = 0;
                    }
                }

                if(emptyPipe)
                {
                    printf("parse error near `|'\n");
                    continue;
                }

                char storeCommands[MAX_SIZE][MAX_SIZE];

                char tempStore[MAX_SIZE];
                strcpy(tempStore, duplicate);

                char *tempCommand = strtok(tempStore, "|");
                int pipeSeparatedCommands = 0;
                while(tempCommand != NULL)
                {
                    // printf("*** %s *** %s\n", tempCommand, duplicate);
                    strcpy(storeCommands[pipeSeparatedCommands], tempCommand);
                    // printf("** %s ** %d\n", storeCommands[pipeSeparatedCommands], pipeSeparatedCommands);
                    pipeSeparatedCommands++;
                    tempCommand = strtok(NULL, "|");
                }

                if(pipeSeparatedCommands == 1)
                {
                    // printf("***\n");
                    int flag = 0;

                    // if(duplicate[0] == '|')
                    // {
                    //     printf("syntax error near `|'\n");
                    //     continue;   
                    // }

                    // for(int k = 0; duplicate2[k] != '\0'; k++)
                    // {
                    //     if(duplicate2[k] == '|')
                    //     {
                    //         flag = 1;
                    //     }
                    // }

                    // if(flag == 1)
                    // {
                    //     printf("Error: empty pipe\n");
                    //     continue;
                    // }

                    for(int k = 0; duplicate2[k] != '\0'; k++)
                    {
                        if(duplicate2[k] == '&')
                        {
                            duplicate2[k] = '\0';
                            duplicate[k] = '\0';
                            flag = 1;                        
                            break;
                        }
                    }

                    char *cur = NULL;

                    int can = 1;

                    if(outputRedirection > 0)
                    {
                        char *input = (char*)malloc(sizeof(char)*MAX_SIZE); 

                        int spaceCount = 0;

                        while(duplicate2[spaceCount] == ' ')
                        {
                            spaceCount++;
                        }

                        for(int k = spaceCount; duplicate2[k] != '\0'; k++)
                        {
                            if(duplicate2[k] == '>' || duplicate2[k] == '<')
                            {
                                input[k] = '\0';
                                break;
                            }

                            input[k-spaceCount] = duplicate2[k];
                        }

                        // strcpy(cur, input);
                        cur = input;

                        if(outputRedirection == 1)
                        {
                            int temp_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                            dup2(temp_fd, 1);
                            close(temp_fd);
                        }

                        else
                        {
                            int temp_fd = open(outputFile, O_WRONLY | O_APPEND | O_CREAT, 0644);
                            dup2(temp_fd, 1);
                            close(temp_fd);   
                        }
                    }

                    // char *cur = strtok(duplicate2, "|");

                    else
                    {
                        // printf("pipeSeparatedCommands = 1 ***\n");
                        cur = strtok(duplicate2, " \t\n");
                    }

                    if(inputRedirection > 0)
                    {
                        char *input = (char*)malloc(sizeof(char)*MAX_SIZE); 

                        int spaceCount = 0;

                        while(duplicate[spaceCount] == ' ')
                        {
                            spaceCount++;
                        }

                        for(int k = spaceCount; duplicate[k] != '\0'; k++)
                        {
                            if(duplicate[k] == '>' || duplicate[k] == '<')
                            {
                                input[k] = '\0';
                                break;
                            }

                            input[k-spaceCount] = duplicate[k];
                        }

                        // strcpy(cur, input);
                        cur = input;
                        // printf("%s\n", cur);
                        // int temp_fd = 0;
                        int temp_fd = open(inputFile, O_RDONLY);
                        // printf("%d\n", temp_fd);
                        if(temp_fd < 0)
                        {
                            can = 0;
                            perror("Wrong input file name");
                            // printf("%s\n", );
                        }

                        else
                        {
                            // printf("*******\n");

                            dup2(temp_fd, 0);
                            close(temp_fd);

                            // printf("%d ****\n", dup2(temp_fd, 0));
                            // printf("%d ***\n", close(temp_fd));
                        }
                    }

                    if(can)
                    {
                        // printf("%d ** %d\n", inputRedirection, outputRedirection);
                        execute(cur, home, proc, &proc_size, data, done, duplicate, flag, outputRedirection, inputRedirection);
                        // fflush(stdout);
                    }

                    dup2(stdin_fd, 0);
                    dup2(stdout_fd, 1);
                }

                // printf("Pipe %d\n", pipeSeparatedCommands);
                else
                {
                    int error = 0;
                    for(int j = 0; j < pipeSeparatedCommands-1; j++)
                    {
                        // printf("%s\n", storeCommands[j]);

                        for(int k = 0; storeCommands[j][k] != '\0'; k++)
                        {
                            if(storeCommands[j][k] == '&')
                            {
                                printf("parse error near `|'\n");
                                error = 1;
                            
                                break;
                            }
                        }

                        if(error == 1)
                        {
                            break;
                        }
                    }

                    if(error == 1)
                    {
                        continue;
                    }

                    int flag = 0;

                    for(int j = 0; storeCommands[pipeSeparatedCommands-1][j] != '\0'; j++)
                    {
                        if(storeCommands[pipeSeparatedCommands-1][j] == '&')
                        {
                            flag = 1;
                            storeCommands[pipeSeparatedCommands-1][j] = '\0';
                            break;
                        }
                    }

                    for(int j = 0; j < pipeSeparatedCommands; j++)
                    {
                        char createCopy[MAX_SIZE];
                        strcpy(createCopy, storeCommands[j]);
                        char *cur = strtok(storeCommands[j], " \t");

                        // printf("** %s\n", createCopy);
                        // printf("%s\n", cur);

                        dup2(curin_fd, 0);
                        close(curin_fd);

                        int can = 1;
                        int checkForInputRedirection = 0;

                        for(int k = 0; createCopy[k] != '\0'; k++)
                        {
                            if(createCopy[k] == '<')
                            {
                                checkForInputRedirection = 1;
                                break;
                            }
                        }
                        if(checkForInputRedirection > 0)
                        {
                            char *input = (char*)malloc(sizeof(char)*MAX_SIZE); 

                            int spaceCount = 0;

                            while(createCopy[spaceCount] == ' ')
                            {
                                spaceCount++;
                            }

                            for(int k = spaceCount; createCopy[k] != '\0'; k++)
                            {
                                if(createCopy[k] == '>' || createCopy[k] == '<')
                                {
                                    input[k] = '\0';
                                    break;
                                }

                                input[k-spaceCount] = createCopy[k];
                            }

                            // strcpy(cur, input);
                            cur = input;
                            // printf("%s\n", cur);
                            // int temp_fd = 0;
                            int temp_fd = open(inputFile, O_RDONLY);
                            // printf("%d\n", temp_fd);
                            if(temp_fd < 0)
                            {
                                can = 0;
                                perror("Wrong input file name");
                                // printf("%s\n", );
                            }

                            else
                            {
                                // printf("*******\n");

                                dup2(temp_fd, 0);
                                close(temp_fd);

                                // printf("%d ****\n", dup2(temp_fd, 0));
                                // printf("%d ***\n", close(temp_fd));
                            }
                        }


                        if(j < pipeSeparatedCommands - 1)
                        {
                            int pipe_fd[2];
                            pipe(pipe_fd);
                            curout_fd = pipe_fd[1];
                            curin_fd = pipe_fd[0];

                            // printf("%d %d ***\n", curout_fd, curin_fd);
                        }
                
                        else
                        {
                            if(outputRedirection > 0)
                            {
                                char *input = (char*)malloc(sizeof(char)*MAX_SIZE); 

                                int spaceCount = 0;

                                while(createCopy[spaceCount] == ' ')
                                {
                                    spaceCount++;
                                }

                                for(int k = spaceCount; createCopy[k] != '\0'; k++)
                                {
                                    if(createCopy[k] == '>')
                                    {
                                        input[k] = '\0';
                                        break;
                                    }

                                    input[k-spaceCount] = createCopy[k];
                                }

                                // strcpy(cur, input);
                                cur = input;
                                // printf("%s * %s\n", input, outputFile);

                                if(outputRedirection == 1)
                                {
                                    int temp_fd = open(outputFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                    curout_fd = dup(temp_fd);
                                    close(temp_fd);
                                }

                                else
                                {
                                    int temp_fd = open(outputFile, O_WRONLY | O_APPEND | O_CREAT, 0644);
                                    curout_fd = dup(temp_fd);
                                    close(temp_fd);   
                                }
                            }

                            else
                            {
                                curout_fd = dup(stdout_fd);
                            }
                        }

                        // printf("** %d\n", curout_fd);
                        dup2(curout_fd, 1);                        
                        close(curout_fd);
                        // printf("11\n");
                        // fflush(stdout);
                        int pid_new = fork();

                        if(pid_new < 0)
                        {
                            perror("Unable to Fork");
                        }

                        else if(pid_new == 0)
                        { 
                            // printf("*** %s\n", cur);

                            if(can && j == pipeSeparatedCommands-1)
                            {
                                execute(cur, home, proc, &proc_size, data, done, createCopy, flag, outputRedirection, checkForInputRedirection);
                            }

                            else if(can)
                            {
                                execute(cur, home, proc, &proc_size, data, done, createCopy, flag, 0, checkForInputRedirection);
                            }
                            
                            exit(0);
                        }

                        else
                        {
                            int status;
                            waitpid(pid_new, &status, 0);
                            dup2(stdin_fd, 0);
                            dup2(stdout_fd, 1);
                        }

                        dup2(stdin_fd, 0);
                        dup2(stdout_fd, 1);
                    }
                }            
            }
        }
    }

    return 0;
}
