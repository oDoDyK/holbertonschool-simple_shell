#include "main.h"

/**
 * get_argument - split command line into words (arguments)
 * @line: input line
 * Return: array of words (args)
 */
char **get_argument(char *line)
{
	int i = 0;
	char *token;
	char **array_command = malloc(sizeof(char *) * MAX_ARGS);

	if (!array_command)
		perror("malloc"), exit(EXIT_FAILURE);

	token = strtok(line, " \t\r\n");
	while (token)
	{
		array_command[i] = malloc(strlen(token) + 1);
		if (!array_command[i])
			perror("malloc"), exit(EXIT_FAILURE);
		strcpy(array_command[i], token);
		token = strtok(NULL, " \t\r\n");
		i++;
	}
	array_command[i] = NULL;
	return (array_command);
}

/**
 * get_path - Find the executable in PATH
 * @line: command name
 * Return: full path if found, or NULL if not
 */
char *get_path(char *line)
{
	char *path, *path_copy, *token, *full_path;
	struct stat st;

	if (!line)
		return (NULL);

	/* If command contains '/', check directly */
	if (strchr(line, '/'))
	{
		if (access(line, X_OK) == 0)
			return (line);
		return (NULL);
	}

	path = _getenv("PATH");
	if (!path || strcmp(path, "") == 0)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
		perror("strdup"), exit(EXIT_FAILURE);

	token = strtok(path_copy, ":");
	while (token)
	{
		full_path = malloc(strlen(token) + strlen(line) + 2);
		if (!full_path)
			perror("malloc"), exit(EXIT_FAILURE);

		sprintf(full_path, "%s/%s", token, line);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		token = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

/**
 * _getenv - searches for environment variable
 * @name: variable name
 * Return: variable value or NULL
 */
char *_getenv(const char *name)
{
	char **env;
	size_t len = strlen(name);

	if (!name || !environ)
		return (NULL);

	for (env = environ; *env != NULL; env++)
		if (strncmp(*env, name, len) == 0 && (*env)[len] == '=')
			return (*env + len + 1);
	return (NULL);
}

/**
 * free_args - free each element of array args
 * @array_command: an array of command
 */
void free_args(char **array_command)
{
	int i;

	if (!array_command)
		return;

	for (i = 0; array_command[i]; i++)
		free(array_command[i]);
	free(array_command);
}

/**
 * execute_command - execute a command using fork and execve
 * @array_command: array of command + args
 * @nbr_command: number of command entered
 */
void execute_command(char **array_command, int nbr_command)
{
	pid_t pid;
	char *command_path = get_path(array_command[0]);

	if (command_path == NULL)
	{
		fprintf(stderr, "./shell: %d: %s: not found\n",
				nbr_command, array_command[0]);
		return;
	}

	pid = fork();
	if (pid == -1)
		perror("fork");
	else if (pid == 0)
	{
		if (execve(command_path, array_command, environ) == -1)
		{
			perror("execve");
			free_args(array_command);
			exit(EXIT_FAILURE);
		}
	}
	else
		wait(NULL);

	if (command_path != array_command[0])
		free(command_path);
}

