#include "simple_shell.h"

/**
 * _strncmp - compare two strings up to n bytes
 * @s1: first string
 * @s2: second string
 * @n: number of bytes to compare
 *
 * Return: 0 if equal, difference otherwise
 */
int _strncmp(u8 *s1, u8 *s2, u64 n)
{
	u64 i;

	for (i = 0; i < n && s1[i] && s2[i]; i++)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
	}
	if (i < n)
		return (s1[i] - s2[i]);
	return (0);
}

