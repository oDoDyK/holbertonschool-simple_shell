#include "main.h"

/**
 * hsh_cd - Change directory
 * @args: Array of tokens
 * @input_stdin: Input from stdin
 * @exit_status: Exit status
 * Return: 1 (success)
 */
int hsh_cd(char **args, char *input_stdin, int *exit_status)
{
	int stat; /* to check chdir() return value */
	char s[128]; /* size of the string s */

	/* get the current working directory, and save it in s */
	getcwd(s, sizeof(s));

	(void)input_stdin;
	(void)exit_status;

	/* we recieve cd without any other argument */
	if (args[1] == NULL)
		/* change to home directory */
		stat = chdir(getenv("HOME"));
	/* we recieve cd - */
	else if (strcmp(args[1], "-") == 0)
		/* change to previous directory */
		stat = chdir(getenv("OLDPWD"));
	/* we recieve cd ~ */
	else if (strcmp(args[1], "~") == 0)
		/* change to home directory */
		stat = chdir(getenv("HOME"));
	else /* change to any path given (absolute or relative) */
		stat = chdir(args[1]);

	/* check if any chdir() syscall had an error */
	if (stat == -1)
		perror("cd had an error");

	/* add the variable OLDPWD to the environment with the value of s */
	/* 1 --> if OLDPWD already exist its value is chaged to s (overwrite) */
	setenv("OLDPWD", s, 1);
	/* ...with the value get from getcwd() */
	/* ...is changed to the value get from getcwd() */
	setenv("PWD", getcwd(s, sizeof(s)), 1);
	return (1);
}

/**
 * hsh_setenv - Change or add and environment variable
 * @args: Array of tokens from input
 * @input_stdin: Input from stdin
 * @exit_status: Exit status
 * Return: Return 1 if use a function, 0 otherwise.
 */
int hsh_setenv(char **args,  char *input_stdin, int *exit_status)
{
	int n_tokens = 0; /* to count how many tokens we recieve (sort of argc) */

	(void)input_stdin;
	(void)exit_status;

	/* get the number of tokens (sort of argc)*/
	while (args[n_tokens] != NULL)
	{
		n_tokens++;
	}

	/* when we have 3 tokens */
	if (n_tokens == 3)
		/**
		 * now we are working with the array of tokens -> args[0] is setenv,
		 * add the variable args[1] to the environment with the value of args[2],
		 * 1 --> if args[1] already exist ist value is changed to args[2] (ovrwrite)
		 */
		setenv(args[1], args[2], 1);

	/* check if we recieve  a number of tokens different than 3 */
	else if (n_tokens != 3)
		fprintf(stderr, "Try use \"setenv [KEY] [VALUE]\"\n");
	/**
	 * else
	 *	fprintf(stderr, "Try use \"setenv [KEY] [VALUE]\"\n");
	 */
	return (1);
}

/**
 * hsh_unsetenv - Delete an environment variable from the environment
 * @args: Array of tokens from input
 * @input_stdin: Input from stdin
 * @exit_status: Exit status
 * Return: Return 1 if use a function, 0 otherwise.
 */
int hsh_unsetenv(char **args,  char *input_stdin, int *exit_status)
{
	int n_tokens = 0; /* to count how many tokens we recieve (sort of argc) */

	(void)input_stdin;
	(void)exit_status;

	/* get the number of tokens (sort of argc) */
	while (args[n_tokens] != NULL)
	{
		n_tokens++;
	}

	/* when we have 2 tokens */
	if (n_tokens == 2)
		/* delete the variable args[1] from the environment */
		unsetenv(args[1]);
	/* check if we recieve a number of tokens different than 2 */
	else if (n_tokens != 2)
		fprintf(stderr, "Try use \"unsetenv [KEY]\"\n");

	return (1);
}

/**
 * hsh_env - Function that print enviromental
 * @args: arguments
 * @input_stdin: input of stdin
 * @exit_status: exit status
 * Return: Always 1 (success)
 */
int hsh_env(char **args, char *input_stdin, int *exit_status)
{
	int i = 0;

	(void)args;
	(void)input_stdin;
	(void)exit_status;


	if (environ[i] == NULL)
	{
		printf("%s", "The built in env is empty");
		return (1);
	}
	for (i = 0; environ[i]; i++)
		printf("%s\n", environ[i]);

	return (1);
}

/**
 * hsh_exit - Function exit built in
 * @args: arguments
 * @input_stdin: input of stdin
 * @exit_status: exit status
 * Return: Exit success
 */
int hsh_exit(char **args, char *input_stdin, int *exit_status)
{
	int output_exit = 0;

	(void)args;
	(void)input_stdin;
	(void)exit_status;

	if (args[1] == NULL)
	{
		free(args);
		free(input_stdin);
		exit(*exit_status);
	}
	if (args[2] != NULL)
	{
		fprintf(stderr, "exit: too many arguments\n");
		return (0);
	}
	output_exit = atoi(args[1]);
	free(args);
	free(input_stdin);
	exit(output_exit);
}
