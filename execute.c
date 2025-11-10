#include "simple_shell.h"
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Execute commands: built-in or external */
int hsh_execute(char **args)
{
    if (args[0] == NULL)
        return 1;

    if (strcmp(args[0], "exit") == 0)
        return hsh_exit(args);  /* call the function, do NOT define it here */

    return launch_process(args);
}

/* Launch external commands */
int launch_process(char **args)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) /* child process */
    {
        if (execvp(args[0], args) == -1)
        {
            perror("hsh");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0) /* fork failed */
    {
        perror("hsh");
    }
    else /* parent process */
    {
        waitpid(pid, &status, WUNTRACED);
    }

    return 1;
}
