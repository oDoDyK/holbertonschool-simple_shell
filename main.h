#ifndef MAIN_H
#define MAIN_H

#define MAX_ARGS 1024

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>

extern char **environ;

char **get_argument(char *line);
char *get_path(char *line);
void free_args(char **array_command);
void execute_command(char **array_command, int nbr_command);
char *_getenv(const char *name);

int main(int argc, char *argv[]);

#endif

