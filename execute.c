#include <string.h>

int hsh_execute(char **args)
{
    if (args[0] == NULL)
        return 1;

    if (strcmp(args[0], "exit") == 0)
        return hsh_exit(args);

    // other built-ins here...

    // otherwise, run external command
    return launch_process(args);
}
