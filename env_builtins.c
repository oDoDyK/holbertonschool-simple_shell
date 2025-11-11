#include "simple_shell.h"

/**
 * env_find_index - find index of a variable in env set
 * @envp: set_t pointer (environment)
 * @name: variable name
 *
 * Return: index (>= 0) if found, -1 otherwise
 */
static i64	env_find_index(set_t *envp, u8 *name)
{
	u64	i;
	u64	j;
	u8	*entry;

	if (envp == 0 || name == 0)
		return (-1);
	for (i = 0; i < envp->size; i++)
	{
		entry = envp->data[i];
		if (entry == 0)
			continue;
		j = 0;
		while (entry[j] && entry[j] != '=' && name[j] && entry[j] == name[j])
			j++;
		if (name[j] == 0 && entry[j] == '=')
			return ((i64) i);
	}
	return (-1);
}

/**
 * shell_setenv_cmd - builtin "setenv"
 * @s: shell_t pointer
 * @args: arguments (["setenv", VAR, VALUE, ...])
 *
 * Return: shell_t pointer
 */
shell_t	*shell_setenv_cmd(shell_t *s, u8 **args)
{
	set_t	*envp;
	set_t	*tmp;
	u8	*buf;
	u64	i, len1, len2;
	i64	idx;

	if (s == 0 || args == 0 || args[0] == 0)
		return (s);
	if (_strlen(args[0]) != _strlen((u8 *) "setenv") ||
		_strcmp(args[0], (u8 *) "setenv") != 0)
		return (s);

	/* usage: setenv VAR VALUE (exactly 2 args) */
	if (args[1] == 0 || args[2] == 0 || args[3] != 0)
	{
		dprintf(STDERR_FILENO, "%s: setenv: invalid number of arguments\n",
			(char *) s->name);
		if (s->exit)
			*(s->exit) = 2;
		return (s);
	}

	envp = s->envp;
	len1 = _strlen(args[1]);
	len2 = _strlen(args[2]);
	buf = (u8 *) malloc(sizeof(u8) * (len1 + 1 + len2 + 1));
	if (buf == 0)
		return (shell_free(s));

	for (i = 0; i < len1; i++)
		buf[i] = args[1][i];
	buf[len1] = '=';
	for (i = 0; i < len2; i++)
		buf[len1 + 1 + i] = args[2][i];
	buf[len1 + 1 + len2] = 0;

	idx = env_find_index(envp, args[1]);
	if (idx >= 0)
	{
		free(envp->data[idx]);
		envp->data[idx] = buf;
	}
	else
	{
		tmp = set_add(envp, buf);
		free(buf);
		if (tmp == 0)
			return (shell_free(s));
		s->envp = tmp;
	}
	if (s->exit)
		*(s->exit) = 0;
	return (s);
}

/**
 * shell_unsetenv_cmd - builtin "unsetenv"
 * @s: shell_t pointer
 * @args: arguments (["unsetenv", VAR, ...])
 *
 * Return: shell_t pointer
 */
shell_t	*shell_unsetenv_cmd(shell_t *s, u8 **args)
{
	set_t	*envp;
	u8	**data;
	u64	i, j;
	i64	idx;

	if (s == 0 || args == 0 || args[0] == 0)
		return (s);
	if (_strlen(args[0]) != _strlen((u8 *) "unsetenv") ||
		_strcmp(args[0], (u8 *) "unsetenv") != 0)
		return (s);

	/* usage: unsetenv VAR (exactly 1 arg) */
	if (args[1] == 0 || args[2] != 0)
	{
		dprintf(STDERR_FILENO, "%s: unsetenv: invalid number of arguments\n",
			(char *) s->name);
		if (s->exit)
			*(s->exit) = 2;
		return (s);
	}

	envp = s->envp;
	idx = env_find_index(envp, args[1]);
	if (idx < 0)
	{
		dprintf(STDERR_FILENO, "%s: unsetenv: %s: not found\n",
			(char *) s->name, (char *) args[1]);
		if (s->exit)
			*(s->exit) = 2;
		return (s);
	}

	if (envp->size == 1)
	{
		free(envp->data[0]);
		free(envp->data);
		envp->data = 0;
		envp->size = 0;
		if (s->exit)
			*(s->exit) = 0;
		return (s);
	}

	data = (u8 **) malloc(sizeof(u8 *) * (envp->size - 1));
	if (data == 0)
		return (shell_free(s));

	j = 0;
	for (i = 0; i < envp->size; i++)
	{
		if (i == (u64) idx)
			continue;
		data[j] = _strdup(envp->data[i]);
		if (data[j] == 0)
		{
			while (j > 0)
			{
				j--;
				free(data[j]);
			}
			free(data);
			return (shell_free(s));
		}
		j++;
	}

	for (i = 0; i < envp->size; i++)
		free(envp->data[i]);
	free(envp->data);

	envp->data = data;
	envp->size = envp->size - 1;

	if (s->exit)
		*(s->exit) = 0;
	return (s);
}

