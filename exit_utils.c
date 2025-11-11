#include "simple_shell.h"

/**
 * validate_exit_arg - Validate and parse the argument of exit
 * @s: shell_t pointer
 * @arg: argument string after "exit"
 * @status: pointer to store numeric exit code
 *
 * Return: 1 if invalid argument, 0 if valid
 */
int validate_exit_arg(shell_t *s, char *arg, int *status)
{
        unsigned int i;
        long val = 0;
        char msg[1024];
        int len;

        if (!arg)
                return (0);

        /* Handle negative numbers */
        if (arg[0] == '-')
        {
                len = sprintf(msg, "%s: 1: exit: Illegal number: %s", s->name, arg);
                write(STDERR_FILENO, msg, len);
                if (s->exit)
                        *(s->exit) = 2;
                return (1);
        }

        /* Ensure all characters are digits */
        for (i = 0; arg[i]; i++)
        {
                if (arg[i] < '0' || arg[i] > '9')
                {
                        len = sprintf(msg, "%s: 1: exit: Illegal number: %s", s->name, arg);
                        write(STDERR_FILENO, msg, len);
                        if (s->exit)
                                *(s->exit) = 2;
                        return (1);
                }
                val = (val * 10 + (arg[i] - '0')) % 256;
        }

        if (status)
                *status = (int)(val % 256);

        return (0);
}

