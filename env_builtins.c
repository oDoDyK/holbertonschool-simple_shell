#include "simple_shell.h"

/**
 * shell_setenv_cmd - Builtin command to set or modify an environment variable
 * @s: shell structure
 * @args: command arguments (expects: setenv VARIABLE VALUE)
 *
 * Description:
 * This function adds or updates an environment variable.
 * If the wrong number of arguments is provided, it prints
 * the correct usage message to stderr.
 *
 * Return: shell pointer
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
	int argc;

	if (!s || !args)
		return (s);

	for (argc = 0; args[argc]; argc++)
		;

	/* Expect exactly 3 arguments: setenv VARIABLE VALUE */
	if (argc != 3)
	{
		fprintf(stderr, "Usage: setenv VARIABLE VALUE\n");
		return (s);
	}

	/* Use custom _setenv instead of system setenv() */
	if (_setenv((char *)args[1], (char *)args[2], 1) == -1)
	{
		fprintf(stderr, "setenv: Failed to set variable\n");
	}

	return (s);
}

/**
 * shell_unsetenv_cmd - Builtin command to remove an environment variable
 * @s: shell structure
 * @args: command arguments (expects: unsetenv VARIABLE)
 *
 * Description:
 * This function removes an environment variable from the environment.
 * If the wrong number of arguments is provided, it prints
 * the correct usage message to stderr.
 *
 * Return: shell pointer
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
	int argc;

	if (!s || !args)
		return (s);

	for (argc = 0; args[argc]; argc++)
		;

	/* Expect exactly 2 arguments: unsetenv VARIABLE */
	if (argc != 2)
	{
		fprintf(stderr, "Usage: unsetenv VARIABLE\n");
		return (s);
	}

	/* Use custom _unsetenv instead of system unsetenv() */
	if (_unsetenv((char *)args[1]) == -1)
	{
		fprintf(stderr, "unsetenv: Failed to unset variable\n");
	}

	return (s);
}

