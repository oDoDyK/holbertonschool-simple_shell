#include "simple_shell.h"
#include <signal.h>
#include <unistd.h>

/**
 * sigint_handler - handle SIGINT (Ctrl + C)
 * @sig: signal number
 *
 * Return: void
 */
void sigint_handler(int sig)
{
	u32 len;

	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	len = _strlen((u8 *)PROMPT_TEXT);
	write(STDOUT_FILENO, PROMPT_TEXT, len);
}

/**
 * shell_set_signal - set handler for SIGINT
 *
 * Return: void
 */
void shell_set_signal(void)
{
	signal(SIGINT, sigint_handler);
}

