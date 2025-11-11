#include "simple_shell.h"

/**
 * env_find_index - find index of VAR in env set
 * @env: pointer to environment set
 * @name: variable name (without '=')
 *
 * Return: index in env->data if found, -1 otherwise
 */
static int env_find_index(set_t *env, u8 *name)
{
 u64 i, j, len;
 u8 *entry;

 if (env == 0 || name == 0)
  return (-1);

 len = _strlen(name);
 for (i = 0; i < env->size; i++)
 {
  entry = env->data[i];
  if (entry == 0)
   continue;

  /* check "NAME=" prefix */
  if (entry[len] != '=')
   continue;

  for (j = 0; j < len; j++)
  {
   if (entry[j] != name[j])
    break;
  }
  if (j == len)
   return ((int)i);
 }
 return (-1);
}

/**
 * env_build_pair - build "NAME=VALUE" string
 * @name: variable name
 * @value: variable value
 *
 * Return: newly allocated "NAME=VALUE" string, or NULL on failure
 */
static u8 *env_build_pair(u8 *name, u8 *value)
{
 u64 len_n, len_v, i, j;
 u8 *s;

 if (name == 0 || value == 0)
  return (0);

 len_n = _strlen(name);
 len_v = _strlen(value);
 s = malloc(len_n + 1 + len_v + 1);
 if (s == 0)
  return (0);

 for (i = 0; i < len_n; i++)
  s[i] = name[i];

 s[i] = '=';
 i++;

 for (j = 0; j < len_v; j++)
  s[i + j] = value[j];

 s[i + j] = '\0';
 return (s);
}

/**
 * shell_setenv_cmd - implements the built-in "setenv" command
 * @s: pointer to shell_t structure
 * @args: command arguments (e.g. {"setenv", "VAR", "VALUE", NULL})
 *
 * Description:
 * - If called with wrong number of args, it does nothing.
 * - With VAR and VALUE, it adds or updates VAR in s->envp.
 *
 * Return: pointer to shell_t structure (s)
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
 int argc, idx;
 u8 *pair;

 if (s == 0 || args == 0)
  return (s);

 for (argc = 0; args[argc]; argc++)
  ;

 /* valid form: setenv VAR VALUE */
 if (argc != 3)
  return (s);

 pair = env_build_pair(args[1], args[2]);
 if (pair == 0)
  return (s);

 idx = env_find_index(s->envp, args[1]);
 if (idx >= 0)
 {
  free(s->envp->data[idx]);
  s->envp->data[idx] = pair;
 }
 else
 {
  set_add(s->envp, pair);
  free(pair);
 }

 return (s);
}

/**
 * shell_unsetenv_cmd - implements the built-in "unsetenv" command
 * @s: pointer to shell_t structure
 * @args: command arguments (e.g. {"unsetenv", "VAR", NULL})
 *
 * Description:
 * - If called with wrong number of args, it does nothing.
 * - With one VAR, it removes that variable from s->envp if present.
 *
 * Return: pointer to shell_t structure (s)
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
 int argc, idx;
 u64 i;

 if (s == 0 || args == 0)
  return (s);

 for (argc = 0; args[argc]; argc++)
  ;

 /* valid form: unsetenv VAR */
 if (argc != 2)
  return (s);

 idx = env_find_index(s->envp, args[1]);
 if (idx < 0)
  return (s);

 /* free this entry and shift others left */
 free(s->envp->data[idx]);
 for (i = (u64)idx; i + 1 < s->envp->size && s->envp->data[i + 1]; i++)
  s->envp->data[i] = s->envp->data[i + 1];

 s->envp->data[i] = 0;
 if (s->envp->size > 0)
  s->envp->size--;

 return (s);
}

