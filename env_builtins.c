#include "simple_shell.h"
#include <stdlib.h>
#include <unistd.h>

/**
* shell_setenv_cmd - Builtin command: setenv VARIABLE VALUE
* @s: shell context
* @args: command arguments
*
* Return: shell context
*/
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
if (!s || !args || !args[1] || !args[2])
{
write(STDERR_FILENO, "setenv: wrong number of arguments\n", 35);
if (s && s->exit)
*(s->exit) = 2;
return (s);
}

/* Update environment variable using the system setenv function */
if (setenv((char *)args[1], (char *)args[2], 1) != 0)
{
perror("setenv");
if (s->exit)
*(s->exit) = 2;
}

return (s);
}

/**
* shell_unsetenv_cmd - Builtin command: unsetenv VARIABLE
* @s: shell context
* @args: command arguments
*
* Return: shell context
*/
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
if (!s || !args || !args[1])
{
write(STDERR_FILENO, "unsetenv: wrong number of arguments\n", 37);
if (s && s->exit)
*(s->exit) = 2;
return (s);
}

/* Remove variable from environment */
if (unsetenv((char *)args[1]) != 0)
{
perror("unsetenv");
if (s->exit)
*(s->exit) = 2;
}

return (s);
}
