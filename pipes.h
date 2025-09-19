#ifndef PIPES_H
#define PIPES_H

#include <stdbool.h>
#include <sys/times.h>

#define MAX_INPUT 1024
#define MAX_ARGS 128
#define MAX_PIPES 128

extern char *args[MAX_ARGS];
extern char *pipes[MAX_PIPES];
extern char input[MAX_INPUT];
extern struct timeval start_time;
extern struct timeval end_time;

extern struct rusage start_usage;
extern struct rusage end_usage;


void split_args(char *entrada);
void split_pipes();
void ejecutar_pipes(int MAX);
bool check(int *numero_pipes);

#endif
