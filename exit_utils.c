#include "simple_shell.h"

/**
 * validate_exit_arg - Check and parse exit argument
 * @s: shell_t pointer
 * @arg: argument string
 * @status: pointer to store parsed value
 *
 * Return: 1 if invalid, 0 if valid
 */
int validate_exit_arg(shell_t *s, char *arg, int *status)
{
	unsigned int i;
	long val = 0;

	if (!arg)
		return (0);

	/* negative number check */
	if (arg[0] == '-')
	{
		fprintf(stderr, "%s: 1: exit: Illegal number: %s\n",
			(char *)s->name, arg);
		if (s->exit)
			*(s->exit) = 2;
		return (1);
	}

	/* non-digit character check */
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

