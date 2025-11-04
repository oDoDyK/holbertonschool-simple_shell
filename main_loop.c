#include "main.h"

/**
 * main - Main loop, recieve input from CLI parse and execute it
 * @argc: Number of arguments passed to the program
 * @argv: Array of arguments
 * Return: Always 0 on success
 */
int main(int argc __attribute__((unused)), char **argv __attribute__((unused)))
{
	char **arguments = NULL; /* array of tokens get from tokenizer */
	char *input_stdin = NULL; /* let getline manage memory, so, set to NULL */
	int status_return = 1, exit_status = 0;
	size_t size = 0; /* let getline manage memory, so, set to 0 */
	ssize_t n = 0; /* to check getline return value and EOF */

	/* check if we stay or exit the loop */
	while (status_return && n != EOF) /* EOF has a value of -1, so it is an int */
	{
		size = 0; /* set to 0 in every new input */
		/* return 1 if the file in the argument refers to the terminal */
		status_return = isatty(STDIN_FILENO);
		if (status_return)
			write(STDOUT_FILENO, "#(ಠ_ಠ)->$ ", 14); /* print the prompt to stdout */
		/* if we recieve Ctrl + c ignore it and print a new line and the prompt */
		signal(SIGINT, sigintH);
		/* store the input_stdin string or check for EOF or any error */
		n = getline(&input_stdin, &size, stdin);
		if (n == -1) /* account for EOF or any error*/
		{
			free(input_stdin);
			break;
		}
		/* validate spaces, tabs and line breaks */
		if (validate_spaces(input_stdin))
		{
			free(input_stdin);
			continue;
		}
		/* split the input_stdin string into a array of arguments (tokens) */
		arguments = hsh_tokenizer(input_stdin);
		if (*arguments[0] == '\0')
			continue;
		/* pass the array of arguments (tokens) to choose from builtin functions */
		status_return = hsh_execute_builtins(arguments, input_stdin,
				argv, &exit_status);
		free(input_stdin);
		free(arguments);
	}
	return (0);
}
