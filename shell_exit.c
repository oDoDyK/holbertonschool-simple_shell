#include "simple_shell.h"

/**
* shell_exit - Cleanly exit the shell (used on EOF, etc.)
* @s: shell_t pointer
* @nl: print newline before exit text if non-zero
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
* parse_status - convert numeric string to exit status (0-255)
* @arg: numeric string (digits only, already validated)
*
* Return: value in range 0-255
*/
static u8 parse_status(char *arg)
{
u32 value = 0;
u32 i = 0;

while (arg[i] != '\0')
{
value = (value * 10 + (arg[i] - '0')) % 256;
i++;
}
return ((u8)value);
}

/**
* shell_exit_cmd - Handle the built-in "exit" command
* @s: shell_t pointer
* @args: arguments array (args[0] == "exit")
*
* Behavior:
*  - exit          -> exit with current status
*  - exit N        -> exit with status N % 256
*  - exit <bad>    -> print "Illegal number" and exit with status 2
*
* Return:
*  - Never returns when args[0] == "exit" (calls exit())
*  - Returns s unchanged when command is not "exit"
*/
shell_t *shell_exit_cmd(shell_t *s, u8 **args)
{
char *arg;
u32 i;
u8 status;

if (s == 0 || args == 0 || args[0] == 0)
return (s);

/* Not the "exit" command */
if (_strcmp(args[0], (u8 *)"exit") != 0)
return (s);

arg = (char *)args[1];

/* ---------- Case 1: exit with no argument ---------- */
if (arg == NULL)
{
status = 0;
if (s->exit)
status = *(s->exit);
shell_free(s);
exit(status);
}

/* ---------- Case 2: invalid number (negative or not digits) ---------- */
if (arg[0] == '-')
goto illegal_number;

for (i = 0; arg[i] != '\0'; i++)
{
if (arg[i] < '0' || arg[i] > '9')
goto illegal_number;
}

/* ---------- Case 3: valid positive integer ---------- */
status = parse_status(arg);
if (s->exit)
*(s->exit) = status;
shell_free(s);
exit(status);

illegal_number:
fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
(char *)s->name, arg);
if (s->exit)
*(s->exit) = 2;
shell_free(s);
exit(2);

/* never reached, but keeps compiler happy */
return (0);
}

