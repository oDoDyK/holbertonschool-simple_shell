#include "simple_shell.h"
#include <stdio.h>
#include <stdlib.h>

/**
* shell_exit - free resources and exit the shell
* @s: shell_t pointer
* @nl: flag to print a newline before exiting
*
* Return: NULL
*/
shell_t *shell_exit(shell_t *s, u8 nl)
{
if (s == NULL)
return (NULL);

if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
{
if (nl)
print_string("\n");
print_string(EXIT_TEXT);
print_string("\n");
}

shell_free(s);
return (NULL);
}

/**
* parse_status - parse string argument to exit status (0–255)
* @arg: argument string (e.g. "98" or "-1")
* @status: pointer to store resulting value
*
* Return: 0 if valid number, -1 otherwise
*/
static int parse_status(u8 *arg, int *status)
{
long value = 0;
int neg = 0;
int i = 0;

if (arg == NULL || status == NULL)
return (-1);

if (arg[0] == '-' || arg[0] == '+')
{
if (arg[0] == '-')
neg = 1;
i++;
}

if (arg[i] == '\0')
return (-1);

for (; arg[i]; i++)
{
if (arg[i] < '0' || arg[i] > '9')
return (-1);
value = value * 10 + (arg[i] - '0');
}

if (neg)
value = -value;

value &= 0xFF;
*status = (int)value;
return (0);
}

/**
* shell_exit_cmd - handle the "exit" built-in command
* @s: shell_t pointer
* @args: arguments array
*
* Return: NULL if shell should terminate, s otherwise
*/
shell_t *shell_exit_cmd(shell_t *s, u8 **args)
{
int status = 0;
u8 *arg;

if (s == NULL || args == NULL || args[0] == NULL)
return (s);

if (_strcmp(args[0], (u8 *)"exit") != 0)
return (s);

arg = args[1];

if (arg != NULL)
{
if (parse_status(arg, &status) != 0)
{
fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
(char *)s->name, (char *)arg);
if (s->exit != NULL)
*(s->exit) = 2;
args[0] = NULL;
return (s);
}
}
else if (s->exit != NULL)
status = *(s->exit);

if (s->exit != NULL)
*(s->exit) = status;

shell_free(s);
exit(status);
}

