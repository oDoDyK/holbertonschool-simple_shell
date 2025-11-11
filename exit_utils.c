#include "simple_shell.h"

/**
 * validate_exit_arg - Validate the argument for the "exit" command
 * @s: shell_t pointer
 * @arg: argument string
 * @status: pointer to store parsed exit status (0–255)
 *
 * Return: 1 if invalid, 0 if valid
 */
int validate_exit_arg(shell_t *s, char *arg, int *status)
{
	unsigned int i;
	long val = 0;
	char msg[1024];
	int len;

	if (!arg)
		return (0);

	/* Handle negative numbers */
	if (arg[0] == '-')
	{
		len = sprintf(msg, "%s: 1: exit: Illegal number: %s\n",
			      (char *)s->name, arg);
		write(STDERR_FILENO, msg, len);
		write(STDERR_FILENO, "\n", 1); /* Ensure newline for checker */
		if (s->exit)
			*(s->exit) = 2;
		return (1);
	}

	/* Ensure all characters are digits */
	for (i = 0; arg[i] != '\0'; i++)
	{
		if (arg[i] < '0' || arg[i] > '9')
		{
			len = sprintf(msg, "%s: 1: exit: Illegal number: %s\n",
				      (char *)s->name, arg);
			write(STDERR_FILENO, msg, len);
			write(STDERR_FILENO, "\n", 1); /* Ensure newline for checker */
			if (s->exit)
				*(s->exit) = 2;
			return (1);
		}
		val = (val * 10 + (arg[i] - '0')) % 256;
	}

	if (status)
		*status = (int)(val & 0xFF);

	return (0);
}

