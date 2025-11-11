#include "simple_shell.h"
#include <string.h>

extern char **environ;

/**
 * _getenv - custom getenv (Holberton allowed)
 * @name: variable name
 *
 * Return: pointer to value string or NULL
 */
char *_getenv(const char *name)
{
	int i;
	size_t len = _strlen((u8 *)name);

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return ((char *)&environ[i][len + 1]);
	}
	return (NULL);
}

