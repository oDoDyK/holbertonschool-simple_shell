#include "simple_shell.h"

/**
 * _strlen_local - simple string length
 * @s: string
 * Return: length
 */
static size_t _strlen_local(const char *s)
{
	size_t n = 0;
	if (!s)
		return (0);
	while (s[n])
		n++;
	return (n);
}

/**
 * _strdup_local - duplicate string safely
 * @s: string
 * Return: new allocated string
 */
static char *_strdup_local(const char *s)
{
	size_t len = _strlen_local(s);
	char *p = malloc(len + 1);
	size_t i;

	if (!p)
		return (NULL);

	for (i = 0; i < len; i++)
		p[i] = s[i];
	p[len] = '\0';
	return (p);
}

/**
 * init_env - duplicate the environment variables
 * @envp: original environment
 * Return: 0 on success, -1 on failure
 */
int init_env(char **envp)
{
	size_t count = 0, i;
	char **copy;

	while (envp[count])
		count++;

	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (-1);

	for (i = 0; i < count; i++)
	{
		copy[i] = _strdup_local(envp[i]);
		if (!copy[i])
		{
			size_t j;
			for (j = 0; j < i; j++)
				free(copy[j]);
			free(copy);
			return (-1);
		}
	}

	copy[count] = NULL;
	environ = copy;
	return (0);
}

/**
 * free_env - free duplicated environment
 */
void free_env(void)
{
	size_t i;

	if (!environ)
		return;

	for (i = 0; environ[i]; i++)
		free(environ[i]);

	free(environ);
	environ = NULL;
}

