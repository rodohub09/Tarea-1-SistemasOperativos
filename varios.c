#include "varios.h"
#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>

extern char *args[MAX_ARGS];

//Funcion para ejecutar comandos simples

void ejecutar(){
  int status;
  pid_t pid;           //Se definen variables y se crea un hijo
  pid = fork();

  start = times(&t);
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
  end = times(&t);
}
