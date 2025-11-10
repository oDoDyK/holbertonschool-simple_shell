#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int hsh_exit(char **args)
{
    int status = 0;

    if (args[1] != NULL)
    {
        char *s = args[1];
        for (int i = 0; s[i]; i++)
        {
            if (!isdigit(s[i]))
            {
                fprintf(stderr, "exit: Illegal number: %s\n", s);
                return 1; // don’t exit yet, just signal error
            }
        }
        status = atoi(s);
    }
    exit(status);
}
