#include "simple_shell.h"

/**
 * shell_exit - Cleanly exit the shell
 * @s: shell_t pointer
 * @nl: print newline before exit text if non-zero
 *
 * Return: NULL after freeing shell resources
 */
shell_t *shell_exit(shell_t *s, u8 nl)
{
	if (!s)
		return (NULL);

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
 * check_exit_argument - validate and parse exit argument
 * @s: shell_t pointer
 * @arg: argument string to check
 * @status: pointer to store parsed value if valid
 *
 * Return: 1 if invalid, 0 if valid
 */
static int check_exit_argument(shell_t *s, char *arg, int *status)
{
	unsigned int i;
	long val = 0;

	if (arg == NULL)
		return (0);

	/* check for negative sign */
	if (arg[0] == '-')
	{
		fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
			(char *)s->name, arg);
		if (s->exit)
			*(s->exit) = 2;
		return (1);
	}

	/* ensure all characters are digits */
	for (i = 0; arg[i]; i++)
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
				(char *)s->name, arg);
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

/**
 * shell_exit_cmd - Handle the built-in "exit" command
 * @s: shell_t pointer
 * @args: arguments array (args[0] == "exit")
 *
 * Return: NULL if shell should terminate, s otherwise
 */
shell_t *shell_exit_cmd(shell_t *s, u8 **args)
{
	int status = 0;

	if (!s || !args || !args[0])
		return (s);

	if (_strcmp(args[0], (u8 *)"exit") != 0)
		return (s);

	/* No argument */
	if (!args[1])
		return (shell_exit(s, 0));

	/* Check argument validity */
	if (check_exit_argument(s, (char *)args[1], &status))
	{
		args[0] = NULL;
		return (s);
	}

	/* Valid numeric argument */
	if (s->exit)
		*(s->exit) = status;

	return (shell_exit(s, 0));
}

