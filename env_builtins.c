#include "simple_shell.h"

extern char **environ;

/**
 * shell_setenv_cmd - Implements the built-in "setenv" command
 * @s: Pointer to shell_t structure
 * @args: Command arguments (e.g. {"setenv", "VAR", "VALUE", NULL})
 *
 * Description:
 * - If no arguments are given or too few/many, it does nothing.
 * - If both VAR and VALUE are provided, it sets/updates the variable.
 * - Returns the shell pointer without exiting.
 *
 * Return: Pointer to shell_t structure (s)
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
 int argc;

 if (s == 0 || args == 0)
  return (s);

 for (argc = 0; args[argc]; argc++)
  ;

 /* Ignore invalid usage (e.g. "setenv" or "setenv VAR" only) */
 if (argc != 3)
  return (s);

 /* Correct usage: setenv VAR VALUE */
 if (setenv((char *)args[1], (char *)args[2], 1) == -1)
  return (s);

 return (s);
}

/**
 * shell_unsetenv_cmd - Implements the built-in "unsetenv" command
 * @s: Pointer to shell_t structure
 * @args: Command arguments (e.g. {"unsetenv", "VAR", NULL})
 *
 * Description:
 * - If no variable name is given, it does nothing.
 * - If the variable exists, it is removed.
 * - Returns the shell pointer.
 *
 * Return: Pointer to shell_t structure (s)
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
 int argc;

 if (s == 0 || args == 0)
  return (s);

 for (argc = 0; args[argc]; argc++)
  ;

 /* Ignore invalid usage (e.g. "unsetenv" with no args) */
 if (argc != 2)
  return (s);

 if (unsetenv((char *)args[1]) == -1)
  return (s);

 return (s);
}

