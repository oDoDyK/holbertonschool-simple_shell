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
 * Return: NULL if shell should terminate, s otherwise
 */
shell_t *shell_exit_cmd(shell_t *s, u8 **args)
{
	char *arg;
	u32 i;

	if (s == 0 || args == 0 || args[0] == 0)
		return (s);

	/* Not the "exit" command */
	if (_strlen(args[0]) != _strlen((u8 *)"exit"))
		return (s);
	if (_strcmp(args[0], (u8 *)"exit") != 0)
		return (s);

	arg = (char *)args[1];

	/* No argument: exit with current status */
	if (arg == NULL)
		return (shell_exit(s, 0));

	/* Negative numbers are considered illegal */
	if (arg[0] == '-')
	{
		fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
			(char *)s->name, arg);
		if (s->exit)
			*(s->exit) = 2;
		/* command اتنفّذ كبِلت إن → لا تنفّذه كـ external */
		args[0] = 0;
		return (s);
	}

	/* Ensure all characters are digits */
	for (i = 0; arg[i] != '\0'; i++)
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
				(char *)s->name, arg);
			if (s->exit)
				*(s->exit) = 2;
			args[0] = 0;
			return (s);
		}
	}

	/* Valid positive integer: compute normalized status (mod 256) */
	if (s->exit)
		*(s->exit) = parse_status(arg);

	return (shell_exit(s, 0));
}

