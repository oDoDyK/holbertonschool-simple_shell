#include "simple_shell.h"

shell_t *shell_prompt(shell_t *s)
{
if (!s)
return (0);
if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
print_string(PROMPT_TEXT);
return (s);
}

shell_t *shell_exec(shell_t *s, u8 *path, u8 **args)
{
pid_t pid;
int status;
u8 **envp;
u64 x;

if (!s)
return (0);
envp = set_consume(set_clone(s->envp));
if (!envp)
return (0);

pid = fork();
if (pid == -1)
{
for (x = 0; envp[x]; x++)
free(envp[x]);
free(envp);
return (0);
}
if (pid == 0)
{
if (execve((char *)path, (char **)args, (char **)envp) == -1)
{
for (x = 0; envp[x]; x++)
free(envp[x]);
free(envp);
return (0);
}
}
else
{
wait(&status);
if (status != 0)
*(s->exit) = 2;
}
for (x = 0; envp[x]; x++)
free(envp[x]);
free(envp);
return (s);
}

shell_t *shell_iter_line(shell_t *s, u8 **args, u64 line)
{
set_t *set;
u8 *str;

if (!s)
return (0);
if (!args)
return (shell_free(s));
if (!args[0])
return (s);

if (shell_exit_cmd(s, args) == 0)
return (0);
if (!args[0])
return (s);

s->path->extra = args[0];
set = set_filter(set_add(set_apply(set_clone(s->path),
set_apply_path_exec), args[0]), set_filter_path_exec);
s->path->extra = 0;

if (!set)
return (shell_free(s));
if (set->size < 1)
{
print_not_found(s->name, line + 1, args[0]);
*(s->exit) = 127;
}
else
{
str = _strdup(set->data[0]);
if (!str)
return (shell_free(s));
if (shell_exec(s, str, args) == 0)
{
print_not_found(s->name, line + 1, args[0]);
*(s->exit) = 127;
}
free(str);
}
set_free(set);
return (s);
}

shell_t *shell_iter(shell_t *s)
{
u8 **lines, **args, *input;
u64 x, y;
u8 flag;

input = read_line();
if (!input)
return (shell_exit(s, 1));
lines = _strsplit(input, (u8 *)";\n");
free(input);
if (!lines)
return (shell_exit(s, 1));

flag = 0;
for (x = 0; lines[x]; x++)
{
args = _strsplit(lines[x], (u8 *)" ");
if (!args)
continue;
flag = (shell_iter_line(s, args, x) == 0);
for (y = 0; args[y]; y++)
free(args[y]);
free(args);
if (flag)
break;
}
for (x = 0; lines[x]; x++)
free(lines[x]);
free(lines);
if (flag)
return (0);
return (s);
}

shell_t *shell_runtime(shell_t *s)
{
if (!s)
return (0);
while (1)
{
if (shell_iter(shell_prompt(s)) == 0)
return (0);
}
return (shell_free(s));
}

