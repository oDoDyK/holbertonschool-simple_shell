#include "simple_shell.h"
#include <signal.h>
#include <unistd.h>

/**
* sigint_handler - handle SIGINT (Ctrl + C)
* @sig: signal number
*
* Description:
* This function prevents the shell from exiting
* when the user presses Ctrl + C.
* It prints a newline and re-displays the prompt.
*/
void sigint_handler(int sig)
{
(void)sig;
write(STDOUT_FILENO, "\n", 1);
write(STDOUT_FILENO, PROMPT_TEXT, _strlen((u8 *)PROMPT_TEXT));
}

/**
* shell_set_signal - setup custom handler for Ctrl + C
*
* Description:
* Registers sigint_handler so that SIGINT is ignored
* and handled gracefully by reprinting the prompt.
*/
void shell_set_signal(void)
{
signal(SIGINT, sigint_handler);
}

