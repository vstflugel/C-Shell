#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <langinfo.h>
#include <sys/wait.h>
#include <signal.h>
#include <ncurses.h>
#include <fcntl.h>

#define MAX_SIZE (int)1e3+5

typedef struct process
{
    int pid;
    char name[MAX_SIZE];
}process;

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"
#define clearScreen() printf("\033[H\033[J")

void prompt();
void cd(char *path, char *home);
void pwd();
void exEcho(char *cur);
void ls(char *token, char *home);
void pinfo(char *token, char *home);
void fg_bg(char *token, int flag, int **proc_size, process proc[]);
int storeHistory(int done, char data[20][MAX_SIZE], char command[]);
void history(char *token, char data[20][MAX_SIZE], int done);
void jobs(process proc[]);
void sig(char *token, process proc[]);
void overkill(process proc[]);
void bg(char *token, process proc[]);
void fg(char *token, process proc[], int **proc_size);
int checkOutputRedirection(char curCommand[], int *outputFileLength, char outputFile[]);
void execute(char *cur, char home[], process proc[], int *proc_size, char data[20][MAX_SIZE], int done, char duplicate[], int flag, int outputRedirection, int inputRedirection);
int checkInputRedirection(char curCommand[], int *inputFileLength, char inputFile[]);
