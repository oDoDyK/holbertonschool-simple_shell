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
 * _unsetenv - remove a variable from environment
 * @name: variable name (no '=')
 * Return: 0 on success, -1 on failure
 */
int _unsetenv(const char *name)
{
	size_t i, j, nlen;

	if (!name || !*name)
		return (-1);

	for (nlen = 0; name[nlen]; nlen++)
		if (name[nlen] == '=')
			return (-1);

	for (i = 0; environ[i]; i++)
	{
		if (_strncmp_local(environ[i], name, nlen) == 0 && environ[i][nlen] == '=')
		{
			free(environ[i]);
			for (j = i; environ[j]; j++)
				environ[j] = environ[j + 1];
			return (0);
		}
	}

	return (0);
}

