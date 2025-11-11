#include "simple_shell.h"

/**
 * _strncmp_local - compares two strings up to n bytes
 * @a: first string
 * @b: second string
 * @n: number of bytes to compare
 * Return: 0 if equal, difference otherwise
 */
static int _strncmp_local(const char *a, const char *b, size_t n)
{
	size_t i;

	if (!a || !b)
		return (a == b ? 0 : (a ? 1 : -1));

	for (i = 0; i < n; i++)
	{
		if (a[i] != b[i] || !a[i] || !b[i])
			return ((unsigned char)a[i] - (unsigned char)b[i]);
	}
	return (0);
}

/**
 * _strlen_local - custom strlen
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
 * build_nv - build NAME=VALUE string
 * @name: variable name
 * @value: variable value
 * Return: new string or NULL on failure
 */
static char *build_nv(const char *name, const char *value)
{
	size_t ln = _strlen_local(name), lv = _strlen_local(value), i;
	char *s = malloc(ln + 1 + lv + 1);

	if (!s)
		return (NULL);

	for (i = 0; i < ln; i++)
		s[i] = name[i];
	s[ln] = '=';
	for (i = 0; i < lv; i++)
		s[ln + 1 + i] = value[i];
	s[ln + 1 + lv] = '\0';

	return (s);
}

/**
 * _setenv - add or replace an environment variable
 * @name: variable name (no '=')
 * @value: variable value
 * @overwrite: 0 = keep existing, 1 = replace
 * Return: 0 on success, -1 on failure
 */
int _setenv(const char *name, const char *value, int overwrite)
{
	size_t i, nlen;
	char *nv;

	if (!name || !*name || !value)
		return (-1);

	for (nlen = 0; name[nlen]; nlen++)
		if (name[nlen] == '=')
			return (-1);

	for (i = 0; environ[i]; i++)
	{
		if (_strncmp_local(environ[i], name, nlen) == 0 && environ[i][nlen] == '=')
		{
			if (!overwrite)
				return (0);

			nv = build_nv(name, value);
			if (!nv)
				return (-1);

			/* Free old variable before replacing to prevent leaks */
			free(environ[i]);
			environ[i] = nv;
			return (0);
		}
	}

	/* Variable not found, add new one */
	{
		size_t count = i, j;
		char **newenv = malloc(sizeof(char *) * (count + 2));

		if (!newenv)
			return (-1);

		for (j = 0; j < count; j++)
			newenv[j] = environ[j];

		nv = build_nv(name, value);
		if (!nv)
		{
			free(newenv);
			return (-1);
		}

		newenv[count] = nv;
		newenv[count + 1] = NULL;

		/* Replace old environ array only (not its contents) */
		free(environ);
		environ = newenv;
	}

	return (0);
}

