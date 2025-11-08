#include "main.h"

/**
 * main - Main loop, recieve input from CLI parse and execute it
 * @argc: Number of arguments passed to the program
 * @argv: Array of arguments
 * Return: Exit status of last command
 */
int main(int argc __attribute__((unused)), char **argv)
{
	char **arguments = NULL;    
	char *input_stdin = NULL;     
	int exit_status = 0;        
	int interactive;             
	size_t size = 0;             
	ssize_t n;                   

	while (1)
	{
		interactive = isatty(STDIN_FILENO);
		if (interactive)
			write(STDOUT_FILENO, "#(ಠ_ಠ)->$ ", 14);

		signal(SIGINT, sigintH);

		n = getline(&input_stdin, &size, stdin);
		if (n == -1)
		{
			free(input_stdin);
			break;      
		}
    
		if (validate_spaces(input_stdin))
		{
			free(input_stdin);
			input_stdin = NULL;
			continue;
		}

		arguments = hsh_tokenizer(input_stdin);
		if (arguments == NULL || arguments[0] == NULL)
		{
			free(input_stdin);
			free(arguments);
			input_stdin = NULL;
			arguments = NULL;
			continue;
		}

		hsh_execute_builtins(arguments, input_stdin, argv, &exit_status);

		free(input_stdin);
		free(arguments);
		input_stdin = NULL;
		arguments = NULL;
	}

	return (exit_status);
}
