#include "simple_shell.h"
#include <unistd.h>
#include <stdlib.h>

extern char **environ;

/**
 * shell_prompt - function
 * @s: shell_t ptr
 *
 * Return: shell_t ptr
 */
shell_t *shell_prompt(shell_t *s)
{
	if (s == 0)
		return (0);
	if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
		print_string(PROMPT_TEXT);
	return (s);
}

/**
 * shell_exec - execute external program
 * @s: shell_t ptr
 * @path: executable path
 * @args: argv array
 *
 * Return: shell_t ptr, or NULL on fatal error
 */
shell_t *shell_exec(shell_t *s, u8 *path, u8 **args)
{
	pid_t pid;
	int status;
	u8 **envp;
	u64 x;

	if (s == 0)
		return (0);

	envp = set_consume(set_clone(s->envp));
	if (envp == 0)
		return (0);

	pid = fork();
	if (pid == -1)
	{
		for (x = 0; envp[x]; x++)
			free(envp[x]);
		free(envp);
		return (0);
	}

	if (pid == 0)
	{
		if (execve((char *)path, (char **)args, (char **)envp) == -1)
		{
			for (x = 0; envp[x]; x++)
				free(envp[x]);
			free(envp);
			return (0);
		}
}
else
{
        wait(&status);

        if (s->exit != NULL)
        {
                /* store real child exit status (like a normal shell) */
                if (WIFEXITED(status))
                        *(s->exit) = WEXITSTATUS(status);
                else if (WIFSIGNALED(status))
                        *(s->exit) = 128 + WTERMSIG(status);
        }
}


	for (x = 0; envp[x]; x++)
		free(envp[x]);
	free(envp);
	return (s);
}

/**
 * shell_iter_line - handle a single command line
 * @s: shell_t ptr
 * @args: arguments array
 * @line: line number
 *
 * Return: shell_t ptr, or NULL if shell should exit
 */

shell_t *shell_iter_line(shell_t *s, u8 **args, u64 line)
{
	set_t *set;
	u8 *str;
	u64 x;

	if (s == 0)
		return (0);
	if (args == 0)
		return (shell_free(s));
	if (args[0] == 0)
		return (s);

	/* Handle "exit" builtin */
	if (shell_exit_cmd(s, args) == 0)
		return (0);

	/* Handle "env" builtin */
	if (_strlen(args[0]) == _strlen((u8 *)"env") &&
	    _strcmp(args[0], (u8 *)"env") == 0)
	{
		for (x = 0; environ[x]; x++)
		{
			print_string(environ[x]);
			print_char('\n');
		}
		return (s);
	}

	/* Handle "setenv" builtin */
	if (_strlen(args[0]) == _strlen((u8 *)"setenv") &&
	    _strcmp(args[0], (u8 *)"setenv") == 0)
	{
		shell_setenv_cmd(s, args);
		return (s);
	}

	/* Handle "unsetenv" builtin */
	if (_strlen(args[0]) == _strlen((u8 *)"unsetenv") &&
	    _strcmp(args[0], (u8 *)"unsetenv") == 0)
	{
		shell_unsetenv_cmd(s, args);
		return (s);
	}

	/* 🧩 Handle logical operators "&&" and "||" */
	for (x = 0; args[x]; x++)
	{
		if (_strcmp(args[x], (u8 *)"&&") == 0 || _strcmp(args[x], (u8 *)"||") == 0)
		{
			u8 **first_cmd, **second_cmd;
			u8 op_and = (_strcmp(args[x], (u8 *)"&&") == 0);
			u64 a, b;

			/* Split first and second command */
			first_cmd = malloc(sizeof(u8 *) * (x + 1));
			if (!first_cmd)
				return (shell_free(s));

			for (a = 0; a < x; a++)
				first_cmd[a] = _strdup(args[a]);
			first_cmd[a] = NULL;

			second_cmd = &args[x + 1];

			/* Execute first command */
			s = shell_iter_line(s, first_cmd, line);
			for (b = 0; first_cmd[b]; b++)
				free(first_cmd[b]);
			free(first_cmd);

			/* Decide if second should run */
			if ((op_and && *(s->exit) == 0) || (!op_and && *(s->exit) != 0))
				s = shell_iter_line(s, second_cmd, line);

			return (s);
		}
	}

	/* Handle external commands via PATH */
	s->path->extra = args[0];
	set = set_filter(
		set_add(
			set_apply(set_clone(s->path), set_apply_path_exec),
			args[0]),
		set_filter_path_exec);
	s->path->extra = 0;

	if (set == 0)
		return (shell_free(s));

	if (set->size < 1)
	{
		print_not_found(s->name, line + 1, args[0]);
		*(s->exit) = 127;
	}
	else
	{
		str = _strdup(set->data[0]);
		if (str == 0)
			return (shell_free(s));
		if (shell_exec(s, str, args) == 0)
		{
			print_not_found(s->name, line + 1, args[0]);
			*(s->exit) = 127;
		}
		free(str);
	}
	set_free(set);
	return (s);
}

/**
 * shell_iter - main loop over commands in a line
 * @s: shell_t ptr
 *
 * Return: shell_t ptr, or NULL if shell should exit
 */

shell_t *shell_iter(shell_t *s)
{
	u8 **l;
	u8 **a;
	u8 *i;
	u64 x, y;
	u8 f;
	char *norm; /* Declare before any code (C90 rule) */

	i = read_line();
	if (i == 0)
		return (shell_exit(s, 1));

	/* Normalize logical operators "&&" and "||" even if no spaces exist */
	norm = normalize_ops((char *)i);
	free(i);
	i = (u8 *)norm;

	/* Split the input line by semicolon or newline */
	l = _strsplit(i, (u8 *)";\n");
	free(i);
	if (l == 0)
		return (shell_exit(s, 1));

	f = 0;
	for (x = 0; l[x]; x++)
	{
		/* Split each command by spaces */
		a = _strsplit(l[x], (u8 *)" ");
		if (a == 0)
			continue;

		/* Execute one command */
		f = (shell_iter_line(s, a, x) == 0);

		/* Free each argument after execution */
		for (y = 0; a[y]; y++)
			free(a[y]);
		free(a);

		/* Stop if shell_iter_line() returns NULL (exit command) */
		if (f)
			break;
	}

	/* Free all commands */
	for (x = 0; l[x]; x++)
		free(l[x]);
	free(l);

	/* Return NULL if shell should exit */
	if (f)
		return (0);
	return (s);
}

/**
 * shell_runtime - entry point loop
 * @s: shell_t ptr
 *
 * Return: shell_t ptr
 */
shell_t *shell_runtime(shell_t *s)
{
	if (s == 0)
		return (0);

	shell_set_signal();

	while (1)
		if (shell_iter(shell_prompt(s)) == 0)
			return (0);

	return (shell_free(s));
}

