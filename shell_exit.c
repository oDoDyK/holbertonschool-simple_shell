#include "simple_shell.h"

/**
* shell_exit - function
* @s: shell_t ptr
* @nl: u8
*
* Return: shell_t ptr
*/
shell_t	*shell_exit(shell_t *s, u8 nl)
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
* shell_exit_cmd - Handle the built-in exit command
* @s: shell data structure
* @args: arguments passed by user
* Return: 0 to stop shell, 1 to continue
*/
int shell_exit_cmd(shell_t *s, u8 **args)
{
int status = 0;
char *arg;

if (args[0] == NULL)
return (1);

if (_strcmp(args[0], (u8 *)"exit") != 0)
return (1);

arg = (char *)args[1];

if (arg != NULL)
{
int i = 0;

while (arg[i])
{
if (arg[i] < '0' || arg[i] > '9')
{
fprintf(stderr, "exit: Illegal number: %s\n", arg);
*(s->exit) = 2;
return (1);
}
i++;
}

status = atoi(arg);
status = status % 256;
*(s->exit) = status;
}
else
{
*(s->exit) = 0;
}

shell_free(s);
exit(*(s->exit));

return (0);
}
