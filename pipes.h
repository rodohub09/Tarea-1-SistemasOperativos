#ifndef PIPES_H
#define PIPES_H

#include <stdbool.h>

#define MAX_INPUT 1024
#define MAX_ARGS 128
#define MAX_PIPES 128

extern char *args[MAX_ARGS];
extern char *pipes[MAX_PIPES];
extern char input[MAX_INPUT];
void split_args(char *entrada);
void split_pipes();
void ejecutar_pipes(int MAX);
bool check(int *numero_pipes);

#endif
