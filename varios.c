#include "varios.h"
#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <string.h>
char *archivo;
int tiempo;
char saveinput[MAX_INPUT];
// Structuras que guardan los tiempos de usuario y de cpu
struct timeval start_time;
struct timeval end_time;

struct rusage start_usage;
struct rusage end_usage;

int status;
pid_t pid;           //Se definen variables y se crea un hijo
long max_mem;

//Funcion para ejecutar comandos simples
void ejecutar(unsigned int limite){
  
  if(limite == 0){
    gettimeofday(&start_time,NULL);
    getrusage(RUSAGE_CHILDREN,&start_usage);

    pid = fork();

    if(pid == 0){
      // Proceso hijo ejecuta el comando
      if(execvp(args[0], args) == -1){
	printf( "Error! , no se reconoce el comando ingresado\n");
	exit(EXIT_FAILURE);
      }
    }
    else if (pid > 0){
      // Proceso padre espera a que termine el hijo
      waitpid(pid, &status, 0);
    }
    else {
      printf("Error al ejecutar el fork");
    }
  
    gettimeofday(&end_time,NULL);
    getrusage(RUSAGE_CHILDREN,&end_usage);
    max_mem = end_usage.ru_maxrss;
  }
  else{
    
    pid = fork();

    if(pid == 0){
      // Proceso hijo ejecuta el comando
      if(execvp(args[0], args) == -1){
	printf( "Error! , no se reconoce el comando ingresado\n");
	exit(EXIT_FAILURE);
      }
    }
    else if (pid > 0){
      signal(SIGALRM, timeout);
      alarm(limite);
      
      // Proceso padre espera a que termine el hijo
      waitpid(pid, &status, 0);
      alarm(0);
    }
    else {
      printf("Error al ejecutar el fork");
    }
    
  }
}

//Funcion que transforma a segundos los tiempos calculados 
void calcular_tiempos(int salida){

    //Se calcula tiempo real 
   double real_time = (end_time.tv_sec - start_time.tv_sec) +
                       (end_time.tv_usec - start_time.tv_usec) / 1e6;

    //Se calcula tiempo de usuario
    double user_time = (end_usage.ru_utime.tv_sec - start_usage.ru_utime.tv_sec) +
                       (end_usage.ru_utime.tv_usec - start_usage.ru_utime.tv_usec) / 1e6;

    //Se calcula tiempo de sistema
    double sys_time = (end_usage.ru_stime.tv_sec - start_usage.ru_stime.tv_sec) +
                      (end_usage.ru_stime.tv_usec - start_usage.ru_stime.tv_usec) / 1e6;

    if(salida == 0){
    // Mostrar resultados
    printf("Tiempo real: %.6f segundos\n", real_time);
    printf("Tiempo de usuario: %.6f segundos\n", user_time);
    printf("Tiempo de sistema: %.6f segundos\n", sys_time);
    printf("Máxima RAM usada: %ld KB\n", max_mem);
    printf("\n");
    }
    else{
      FILE *results = fopen(archivo, "r");

      if(results){
	fclose(results);
	results = fopen(archivo, "a");
      }
      else {
	results = fopen(archivo, "w");
      }
      
      if(results == NULL){
	printf("No se pudo crear el archivo\n");
	return;
    }
      fprintf(results,"Comando: %s\n", saveinput);
      fprintf(results,"Tiempo real: %.6f segundos\n", real_time);
      fprintf(results,"Tiempo de usuario: %.6f segundos\n", user_time);
      fprintf(results,"Tiempo de sistema: %.6f segundos\n", sys_time);
      fprintf(results,"Máxima RAM usada: %ld KB\n", max_mem);
      fprintf(results,"\n");
      fclose(results);
    }
}

void timeout(int sig){
  printf("Tiempo limite superado, el proceso ha sido terminado\n");
  kill(pid, SIGKILL);
}

void ejec_miprof(int miprof){
  switch(miprof){
  case 1:
    calcular_tiempos(0);
    break;
  case 2:
    calcular_tiempos(1);
    break;
  }
}

int check_miprof(){
  char aux[MAX_INPUT];
  strcpy(aux, input);
  char *arg = strtok(aux, " ");
  
  if(strcmp(arg,"miprof") != 0){
    return 0;
  }
  else{
    arg = strtok(NULL, " ");
    if(strcmp(arg,"ejec") == 0){
      memmove(input,input+12,strlen(input+12)+1);
      return 1;
    }
    else if(strcmp(arg,"ejecsave") == 0){
      archivo = strtok(NULL, " ");
      memmove(input,input+16+strlen(archivo)+1,strlen(input+16+strlen(archivo)+1)+1);
      strcpy(saveinput, input);
      return 2;
    }
    else if(strcmp(arg,"ejecutar") == 0){
      char *auxtiempo = strtok(NULL, " ");
      memmove(input,input+16+strlen(auxtiempo)+1, strlen(input+16+strlen(auxtiempo)+1)+1);
      tiempo = (unsigned int) atoi(auxtiempo);
      return 3;
    }
    else{
      printf("Error al ingresar comando miprof\n");
      return 4;
    } 
  }
}      
