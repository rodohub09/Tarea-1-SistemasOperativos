#include "varios.h"
#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/resource.h>

char *archivo;
int tiempo;
extern char *args[MAX_ARGS];

// Structuras que guardan los tiempos de usuario y de cpu
struct timeval start_time;
struct timeval end_time;

struct rusage start_usage;
struct rusage end_usage;


//Funcion para ejecutar comandos simples
void ejecutar(){
  int status;
  pid_t pid;           //Se definen variables y se crea un hijo

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
    printf("");
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
      fprintf(results,"Comando: %s\n", input);
      fprintf(results,"Tiempo real: %.6f segundos\n", real_time);
      fprintf(results,"Tiempo de usuario: %.6f segundos\n", user_time);
      fprintf(results,"Tiempo de sistema: %.6f segundos\n", sys_time);
      fprintf(results,"\n");
      fclose(results);
    }
}
