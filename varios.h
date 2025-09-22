#ifndef VARIOS_H
#define VARIOS_H

#include <sys/types.h>

extern char *archivo;
extern int tiempo;
extern char *args[];
extern struct timeval start_time;
extern struct timeval end_time;

extern struct rusage start_usage;
extern struct rusage end_usage;

void ejecutar();
void calcular_tiempos();
#endif
