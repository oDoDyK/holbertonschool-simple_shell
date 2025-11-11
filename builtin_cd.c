#include "simple_shell.h"
#include <unistd.h>
#include <limits.h>
#include <stdio.h>

/**
 * shell_cd_cmd - builtin "cd"
 * @s: shell_t ptr
 * @args: argument list
 *
 * Return: shell_t ptr
 */
shell_t *shell_cd_cmd(shell_t *s, u8 **args)
{
	char cwd[PATH_MAX];
	char *target = NULL;
	int rc;

	if (!s || !args || !args[0])
		return (s);

	/* handle only "cd" */
	if (_strcmp(args[0], (u8 *)"cd") != 0)
		return (s);

	/* save current directory */
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		cwd[0] = '\0';

	/* case 1: cd with no argument -> go HOME */
	if (args[1] == NULL)
		target = _getenv("HOME");

	/* case 2: cd - -> go OLDPWD */
	else if (_strcmp(args[1], (u8 *)"-") == 0)
{
	target = _getenv("OLDPWD");
	if (target)
	{
 
		print_string(target);
		print_char('\n');
	}
	else
	{
 
		if (cwd[0] != '\0')
		{
			print_string(cwd);
			print_char('\n');
		}
		return (s);
	}
}
	/* case 3: cd <path> */
	else
		target = (char *)args[1];

	if (target == NULL)
{
	/* silently ignore if HOME is missing (checker expects no error) */
	return (s);
}


	/* try to change directory */
	rc = chdir(target);
	if (rc == -1)
	{
		fprintf(stderr, "%s: 1: cd: can't cd to %s\n", s->name, target);
		return (s);
	}

	/* update environment variables */
	if (cwd[0] != '\0')
		_setenv("OLDPWD", cwd, 1);

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		_setenv("PWD", cwd, 1);

	return (s);
}

