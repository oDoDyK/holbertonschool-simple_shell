#include "simple_shell.h"
#include <errno.h>


/**
 * shell_setenv_cmd - Builtin command: setenv VARIABLE VALUE
 * @s: shell structure
 * @args: command arguments
 * Return: shell pointer
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
	int argc;

	if (!s || !args)
		return (s);

	for (argc = 0; args[argc]; argc++)
		;

	if (argc != 3)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (s);
	}

	if (setenv((char *)args[1], (char *)args[2], 1) == -1)
	{
		perror("setenv");
	}

	return (s);
}

/**
 * shell_unsetenv_cmd - Builtin command: unsetenv VARIABLE
 * @s: shell structure
 * @args: command arguments
 * Return: shell pointer
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
	int argc;

	if (!s || !args)
		return (s);

	for (argc = 0; args[argc]; argc++)
		;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (s);
	}

	if (unsetenv((char *)args[1]) == -1)
	{
		perror("unsetenv");
	}

	return (s);
}

