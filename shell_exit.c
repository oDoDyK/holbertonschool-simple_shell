#include "simple_shell.h"

/**
* shell_exit - Cleanly exit the shell
* @s: shell_t pointer
* @nl: if non-zero, print a newline before exit text
*
* Return: NULL after freeing shell resources
*/
shell_t *shell_exit(shell_t *s, u8 nl)
{
if (s == 0)
return (0);

if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
{
if (nl)
print_string("\n");
print_string(EXIT_TEXT);
print_string("\n");
}

return (shell_free(s));
}

/**
* shell_exit_cmd - Handle the built-in "exit" command
* @s: shell_t pointer
* @args: arguments array (args[0] == "exit")
*
* Return: NULL if shell should terminate, s otherwise
*/
shell_t *shell_exit_cmd(shell_t *s, u8 **args)
{
char *arg;
long status = 0;
int i;

if (s == 0 || args == 0 || args[0] == 0)
return (s);

if (_strlen(args[0]) != _strlen((u8 *)"exit"))
return (s);
if (_strcmp(args[0], (u8 *)"exit") != 0)
return (s);

arg = (char *)args[1];

if (arg != NULL)
{
if (arg[0] == '-')
{
fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
(char *)s->name, arg);
if (s->exit)
*(s->exit) = 2;
return (s);
}

for (i = 0; arg[i]; i++)
{
if (arg[i] < '0' || arg[i] > '9')
{
fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
(char *)s->name, arg);
if (s->exit)
*(s->exit) = 2;
return (s);
}
}

status = atoi(arg);
if (s->exit)
*(s->exit) = (unsigned char)status;

return (shell_exit(s, 0));
}

return (shell_exit(s, 0));
}

