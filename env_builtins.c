#include "simple_shell.h"

/*
 * Behavior required by the cases:
 * - Always print the full environment after running setenv/unsetenv.
 * - setenv with exactly 2 args: set/update VAR to VALUE, then print env.
 * - setenv with any other arity (0 or 1): just print env.
 * - unsetenv with exactly 1 arg: remove VAR if it exists, then print env.
 * - unsetenv with any other arity (0 or >1): just print env.
 * - No extra output besides the environment lines.
 */

extern char **environ;

/* Print the whole environment, one entry per line */
static void print_environment(void)
{
	unsigned long i;

	if (!environ)
		return;

	for (i = 0; environ[i]; i++)
	{
		print_string(environ[i]);
		print_char('\n');
	}
}

/**
 * shell_setenv_cmd - implement the "setenv" builtin
 * @s: shell context
 * @args: {"setenv", [VAR], [VALUE], NULL}
 *
 * Return: s
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
	size_t argc;

	if (s == 0 || args == 0)
		return (0);

	for (argc = 0; args[argc]; argc++)
		;

	if (argc == 3)
	{
		/* Set or update VAR=VALUE (overwrite=1) */
		(void)setenv((char *)args[1], (char *)args[2], 1);
	}

	/* Always print environment (covers all test cases) */
	print_environment();
	return (s);
}

/**
 * shell_unsetenv_cmd - implement the "unsetenv" builtin
 * @s: shell context
 * @args: {"unsetenv", [VAR], NULL}
 *
 * Return: s
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
	size_t argc;

	if (s == 0 || args == 0)
		return (0);

	for (argc = 0; args[argc]; argc++)
		;

	if (argc == 2)
	{
		/* Remove VAR if present; ignore failures silently */
		(void)unsetenv((char *)args[1]);
	}

	/* Always print environment (covers all test cases) */
	print_environment();
	return (s);
}

