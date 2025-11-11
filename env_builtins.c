#include "simple_shell.h"

/**
 * _is_valid_var_name - Check if variable name is valid
 * @name: variable name
 *
 * Return: 1 if valid, 0 otherwise
 */
static int _is_valid_var_name(u8 *name)
{
    u64 i;

    if (name == NULL || name[0] == '\0')
        return (0);

    if (!((name[0] >= 'a' && name[0] <= 'z') ||
          (name[0] >= 'A' && name[0] <= 'Z') ||
          name[0] == '_'))
        return (0);

    for (i = 1; name[i] != '\0'; i++)
    {
        if (!((name[i] >= 'a' && name[i] <= 'z') ||
              (name[i] >= 'A' && name[i] <= 'Z') ||
              (name[i] >= '0' && name[i] <= '9') ||
              name[i] == '_'))
            return (0);
    }
    return (1);
}

/**
 * _getenv_index - Get index of environment variable
 * @s: shell_t pointer
 * @name: variable name
 *
 * Return: index if found, -1 if not found
 */
static int _getenv_index(shell_t *s, u8 *name)
{
    u64 i, name_len;

    if (s == NULL || s->envp == NULL || name == NULL)
        return (-1);

    name_len = _strlen(name);
    for (i = 0; i < s->envp->size; i++)
    {
        if (_strncmp(s->envp->data[i], name, name_len) == 0 &&
            s->envp->data[i][name_len] == '=')
            return (i);
    }
    return (-1);
}

/**
 * _update_system_env - Update system environ from shell envp
 * @s: shell_t pointer
 */
static void _update_system_env(shell_t *s)
{
    u64 i, j;
    extern char **environ;

    if (s == NULL || s->envp == NULL)
        return;

    /* Free old environ if it exists */
    if (environ != NULL)
    {
        for (i = 0; environ[i] != NULL; i++)
            free(environ[i]);
        free(environ);
    }

    /* Create new environ from shell envp */
    environ = malloc(sizeof(char *) * (s->envp->size + 1));
    if (environ == NULL)
        return;

    for (i = 0; i < s->envp->size; i++)
    {
        environ[i] = (char *)_strdup(s->envp->data[i]);
        if (environ[i] == NULL)
        {
            /* Cleanup on failure */
            for (j = 0; j < i; j++)
                free(environ[j]);
            free(environ);
            environ = NULL;
            return;
        }
    }
    environ[i] = NULL;
}

/**
 * shell_setenv_cmd - Implement setenv builtin command
 * @s: shell structure
 * @args: command arguments
 *
 * Return: shell pointer
 */
shell_t *shell_setenv_cmd(shell_t *s, u8 **args)
{
    int argc, index;
    u8 *new_var;
    u64 name_len, value_len, total_len;

    if (s == NULL || args == NULL)
        return (s);

    for (argc = 0; args[argc] != NULL; argc++)
        ;

    if (argc != 3)
    {
        fprintf(stderr, "%s: setenv: Usage: setenv VARIABLE VALUE\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    if (!_is_valid_var_name(args[1]))
    {
        fprintf(stderr, "%s: setenv: Invalid variable name\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    name_len = _strlen(args[1]);
    value_len = _strlen(args[2]);
    total_len = name_len + value_len + 2;

    new_var = malloc(total_len);
    if (new_var == NULL)
    {
        fprintf(stderr, "%s: setenv: Memory allocation failed\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    _strcpy(new_var, args[1]);
    new_var[name_len] = '=';
    _strcpy(new_var + name_len + 1, args[2]);

    index = _getenv_index(s, args[1]);
    if (index >= 0)
    {
        free(s->envp->data[index]);
        s->envp->data[index] = new_var;
    }
    else
    {
        set_t *new_envp = set_add(s->envp, new_var);
        free(new_var);
        if (new_envp == NULL)
        {
            fprintf(stderr, "%s: setenv: Failed to add variable\n",
                    (char *)s->name);
            if (s->exit != NULL)
                *(s->exit) = 2;
            return (s);
        }
        s->envp = new_envp;
    }

    _update_system_env(s);
    return (s);
}

/**
 * shell_unsetenv_cmd - Implement unsetenv builtin command
 * @s: shell structure
 * @args: command arguments
 *
 * Return: shell pointer
 */
shell_t *shell_unsetenv_cmd(shell_t *s, u8 **args)
{
    int argc, index;
    u64 i;

    if (s == NULL || args == NULL)
        return (s);

    for (argc = 0; args[argc] != NULL; argc++)
        ;

    if (argc != 2)
    {
        fprintf(stderr, "%s: unsetenv: Usage: unsetenv VARIABLE\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    if (!_is_valid_var_name(args[1]))
    {
        fprintf(stderr, "%s: unsetenv: Invalid variable name\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    index = _getenv_index(s, args[1]);
    if (index < 0)
    {
        fprintf(stderr, "%s: unsetenv: Variable not found\n",
                (char *)s->name);
        if (s->exit != NULL)
            *(s->exit) = 2;
        return (s);
    }

    /* Remove variable from envp */
    free(s->envp->data[index]);
    for (i = index; i < s->envp->size - 1; i++)
    {
        s->envp->data[i] = s->envp->data[i + 1];
    }
    s->envp->size--;
    s->envp->data[s->envp->size] = NULL;

    _update_system_env(s);
    return (s);
}

