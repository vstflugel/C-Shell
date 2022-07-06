### `This is the readme file which will contains all the instructions and abouts of my shell.`

### How to execute the shell

1. make
2. ./a.out

### Files

    * main.c        - contains the main functioning of the shell
    * def.h         - contains all the required headers and functions
    * prompt.c      - to display the prompt to user
    * cd.c          - cd command
    * pinfo.c       - pinfo command
    * echo.c        - echo command
    * pwd.c         - pwd command
    * ls.c          - ls command
    * fg_bg.c       - handling background and foreground processes for commands which are executed through execvp
    * history.c     - history command
    * execute.c     - helper file which executes the commands
    * bg.c          - bg command
    * fg.c          - fg command
    * jobs.c        - jobs command
    * redirection.c - helper file for executing input and output redirection

### Commands and their executions

    * cd <directory path>                       - Same as normal terminal
    * pwd                                       - Shows the absolute path of the current working directory
    * echo                                      - Prints the message to the terminal.
    * ls [al] <directory path>                  - Handles all variations of ls with a and l flags and directory path
    * jobs                                      - prints list of all currently running background jobs along with their pid and their current state
    *   sig<jobNumber> <signalNumber>           - takes the job id of a running job and sends a signal value to that process
    * fg <jobNumber>                            - brings a running or a stopped background job with a given job number to background
    * bg <jobNumber>                            - changes a stopped background job to a running background job
    * CTRL-D                                    - closing the shell
    * CTRL-Z                                    - changes the status of currently running job to stop, and pushes it in background process
    * CTRL-C                                    - cause a SIGINT signal to be sent to the current foreground job of this shell.
    * input-output redirection functionality    - Same as normal terminal
    * piping functionality                      - Same as normal terminal

