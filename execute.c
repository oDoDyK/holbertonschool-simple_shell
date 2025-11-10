#include "simple_shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int launch_process(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) // child
    {
        if (execvp(args[0], args) == -1)
        {
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) // fork failed
    {
        perror("hsh");
    }
    else // parent
    {
        // wait for child to finish
        waitpid(pid, &status, WUNTRACED);
    }

    return 1;
}
