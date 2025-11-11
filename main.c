#include "simple_shell.h"

/**
 * main - entry point
 * @argc: int
 * @argv: arguments
 * @envp: environment
 * Return: exit status
 */
int main(int argc, char **argv, char **envp)
{
	int exit;
	shell_t *shell;

	exit = 0;

	/* Duplicate environment before starting shell */
	if (init_env(envp) == -1)
		return (1);

	shell = shell_init(0, (u8 *)argv[0], envp);
	if (shell == 0)
	{
		free_env();
		return (0);
	}

	shell->exit = &exit;
	shell_free(shell_runtime(shell));

	/* Free duplicated environment at the end */
	free_env();

	(void)argc;
	return (exit);
}

