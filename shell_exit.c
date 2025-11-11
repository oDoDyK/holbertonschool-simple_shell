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

	/* Invalid argument (string or negative) */
	if (validate_exit_arg(s, (char *)args[1], &status))
	{
		args[0] = NULL;
		return (s); /* Don't exit shell, just continue */
	}

	/* Valid numeric argument */
	if (s->exit)
		*(s->exit) = status;

	return (shell_exit(s, 0));
}

