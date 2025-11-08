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
	int i = 0;
	int sign = 1;

	if (s == 0 || args == 0 || args[0] == 0)
		return (s);

	/* Not the "exit" command, continue normally */
	if (_strlen(args[0]) != _strlen((u8 *)"exit"))
		return (s);
	if (_strcmp(args[0], (u8 *)"exit") != 0)
		return (s);

	arg = (char *)args[1];

	/* If argument is provided, validate and compute status code */
	if (arg != NULL)
	{
		if (arg[0] == '-')
		{
			sign = -1;
			i = 1;
		}

		for (; arg[i]; i++)
		{
			if (arg[i] < '0' || arg[i] > '9')
			{
				/* Illegal number: do not exit, just set status 2 */
				fprintf(stderr, "exit: Illegal number: %s\n", arg);
				if (s->exit)
					*(s->exit) = 2;
				return (s);
			}
			status = status * 10 + (arg[i] - '0');
		}

		status *= sign;

		/* Normalize to 0–255 like real shells (supports >255 and negatives) */
		if (s->exit)
			*(s->exit) = (unsigned char)status;
	}

	/* No argument: keep current *(s->exit) (last command status) */
	return (shell_exit(s, 0));
}

