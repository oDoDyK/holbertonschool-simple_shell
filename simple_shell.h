#ifndef SIMPLE_SHELL_H
# define SIMPLE_SHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdint.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <signal.h>

extern char **environ;

# define i8  int8_t
# define u8  uint8_t
# define i32 int32_t
# define u32 uint32_t
# define i64 int64_t
# define u64 uint64_t

/**
 * struct vector_s - dynamic byte buffer
 * @data: byte array
 * @size: total allocated
 * @curr: current used size
*/
typedef struct vector_s
{
	u8	*data;
	u64	size;
	u64	curr;
}	vector_t;

/**
 * struct set_s - dynamic array of strings
 * @data: array of string pointers
 * @extra: helper pointer
 * @size: number of strings
*/
typedef struct set_s
{
	u8	**data;
	u8	*extra;
	u64	size;
}	set_t;

/**
 * struct shell_s - shell context
 * @exit: pointer to exit status
 * @name: program name
 * @envp: environment variables
 * @path: PATH directories
*/
typedef struct shell_s
{
	int	*exit;
	u8	*name;
	set_t	*envp;
	set_t	*path;
}	shell_t;

/* String functions */
u8		*_strdup(u8 *);
u8		*_strchr(u8 *, u8);
u64		_strlen(u8 *);
i32		_strcmp(u8 *, u8 *);
u8		*_strcat(u8 *, u8 *);
u8		**_strsplit(u8 *str, u8 *lim);
int		_strncmp(u8 *s1, u8 *s2, u64 n);
u8		*_strcpy(u8 *dest, u8 *src);

/* I/O functions */
void		print_char(char);
void		print_string(char *);
void		print_not_found(u8 *, u64, u8 *);
void		free_string_array(u8 **);
u8		*read_line(void);

/* Vector functions */
vector_t	*vector_new(vector_t *);
vector_t	*vector_free(vector_t *);
vector_t	*vector_write(vector_t *, void *src, u64 len);
vector_t	*vector_read(vector_t *, void *dst, u64 size);
u8		*vector_consume(vector_t *);

/* Set functions */
set_t		*set_new(set_t *);
set_t		*set_free(set_t *);
u8		**set_consume(set_t *);
set_t		*set_add(set_t *, u8 *str);
set_t		*set_clone(set_t *);
set_t		*set_filter(set_t *, u8 (*fn)(set_t *, u8 *));
set_t		*set_apply(set_t *, vector_t *(*fn)(set_t *, vector_t *));
u8		set_filter_path(set_t *, u8 *);
vector_t	*set_apply_path(set_t *, vector_t *);
u8		set_filter_path_exec(set_t *, u8 *);
vector_t	*set_apply_path_exec(set_t *, vector_t *);

/* Shell core functions */
shell_t		*shell_new(shell_t *, u8 *name, set_t *envp, set_t *path);
shell_t		*shell_init_return(shell_t *s, set_t *envp, set_t *path);
shell_t		*shell_init(shell_t *, u8 *name, char **env);
shell_t		*shell_free(shell_t *);
shell_t		*shell_prompt(shell_t *);
shell_t		*shell_exec(shell_t *, u8 *path, u8 **args);
shell_t		*shell_exit(shell_t *, u8 nl);
shell_t		*shell_iter_line(shell_t *, u8 **args, u64 line);
shell_t		*shell_iter(shell_t *);
shell_t		*shell_runtime(shell_t *);

/* Built-in commands */
shell_t		*shell_exit_cmd(shell_t *, u8 **args);
shell_t		*shell_setenv_cmd(shell_t *, u8 **args);
shell_t		*shell_unsetenv_cmd(shell_t *, u8 **args);
int _setenv(const char *name, const char *value, int overwrite);
int _unsetenv(const char *name);

/* Signal handling */
void		sigint_handler(int sig);
void		shell_set_signal(void);

/* Legacy functions (keep for compatibility) */
int		hsh_execute(char **args);
int		hsh_exit(char **args);
int		launch_process(char **args);

int init_env(char **envp);
void free_env(void);

/* Constants */
# define BUFFER_SIZE (1024)
# define EXIT_TEXT   "👋"
# define PROMPT_TEXT "🥷 $ "
# define ERRFILE     "No such file or directory\n"
# define ERRFOUND    "not found\n"

#endif

