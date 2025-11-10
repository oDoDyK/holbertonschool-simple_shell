#include "simple_shell.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/* Built-in exit command */
int hsh_exit(char **args)
{
    int status = 0;

    if (args[1] != NULL)
    {
        char *s = args[1];
        int i;  /* loop variable declared before loop (C90) */
        for (i = 0; s[i]; i++)
        {
            if (!isdigit(s[i]))
            {
                fprintf(stderr, "exit: Illegal number: %s\n", s);
                return 1;  /* don’t exit yet, just signal error */
            }
        }
        status = atoi(s);
    }
    exit(status);
}
