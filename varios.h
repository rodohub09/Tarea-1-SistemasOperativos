#ifndef VARIOS_H
#define VARIOS_H

#include "pipes.h"
#include <sys/types.h>

extern char *archivo;
extern int tiempo;
extern char saveinput[MAX_INPUT];
extern struct timeval start_time;
extern struct timeval end_time;

extern struct rusage start_usage;
extern struct rusage end_usage;

void ejecutar(unsigned int limite);
void calcular_tiempos();
void timeout(int sig);
#endif
