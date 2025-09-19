#include "pipes.h"
#include "varios.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sys/resource.h>

char *args[MAX_ARGS];      //Array de strings usado para guardar un comando y sus argumentos 
char *pipes[MAX_PIPES];    //Array de strings usado cuando hay pipes para guardar cada comando y argumentos en cada espacio
char input[MAX_INPUT];     //String donde se guarda la input principal
//Tokeniza cierta string por cada espacio encontrado, cada subcadena la guarda en "args"
struct tms t;
clock_t start, end;

void split_args(char *entrada){
  int i = 0;
  args[i] = strtok(entrada, " ");
  while(args[i] != NULL && i < MAX_ARGS -1){
    args[++i] = strtok(NULL, " ");
  }
  args[i] = NULL;
}


//Tokeniza el input principal por cada simbolo "|" encontrado y guarda cada pipe en un array aparte

void split_pipes(){
  int i = 0;
  int j = 1;
  pipes[i] = strtok(input,"|");
  while(pipes[i] != NULL && i<MAX_PIPES -1){
    pipes[++i] = strtok(NULL,"|");
  }
  pipes[i] = NULL;
  while(pipes[j]!=NULL){
    memmove(pipes[j], pipes[j]+1, strlen(pipes[j])+1);
    j++;
  }
}



//Funcion utilizada para ejecutar los comandos en caso de existir pipes 

void ejecutar_pipes(int MAX){  // Recibe la cantidad de procesos a ejecutar
  
  int c_pipes[MAX][2];         /*Se crea una matriz de la cantidad de proces x 2, en particular
				 c_pipes[MAX][0] = lectura y c_pipes[MAX][1] = escritura */
				 
  pid_t pid;

  gettimeofday(&start_time,NULL);
  getrusage(RUSAGE_SELF,&start_usage);
  
  for(int i = 0; i< MAX; i++){   // Se ejecuta un ciclo creando cada pipe
    if(pipe(c_pipes[i]) == -1){
      printf("Error al ejecutar el pipe");
      exit(EXIT_FAILURE);
    }
  }
		
  for(int i = 0; i < MAX;i++){     //Ciclo principal de ejecucion
    
    pid = fork();                  // Crea un hijo

    
    if(pid == 0){    //Proceso hijo

      // Si no es el primer comando, toma el pipe anterior y lo lee
      if(i>0){
	dup2(c_pipes[i-1][0], STDIN_FILENO);
      }

      // Si no es el ultimo comando, envia la salida al pipe actual y escribe en el
      if(i < MAX - 1){
	dup2(c_pipes[i][1], STDOUT_FILENO);
      }
      
      //Cerramos los pipes activos, para evitar errores de lectura
      
      for(int j = 0; j < MAX -1; j++){
	close(c_pipes[j][0]);
	close(c_pipes[j][1]);
      }

      // Llama a la funcion split para cada tokenizar cada pipe con sus respectivos argumentos
      split_args(pipes[i]);

      //Ejecutamos el pipe correspondiente
      if(execvp(args[0],args) == -1){
	printf( "Error! , no se reconoce el comando ingresado\n");
	exit(EXIT_FAILURE);
      }	
    }
  }
   //Cerramos todos los pipes del padre ya que no necesita leer ni escribir
   for(int i = 0; i < MAX - 1; i++){
     close(c_pipes[i][0]);
     close(c_pipes[i][1]);
   }

   //El padre espera por el hijo en cada iteracion
   for(int i = 0; i < MAX; i++){
     wait(NULL);
  }
  gettimeofday(&end_time,NULL);
  getrusage(RUSAGE_SELF,&end_usage);
}
  

//Funcion que revisa si existen pipes, devuelve un booleano y cuenta cuantos hay
bool check(int *numero_pipes){
  for(int i = 0; i<strlen(input); i++){
    if(input[i] == '|'){
      (*numero_pipes)++;
    }
  }
  if((*numero_pipes) > 0 ){
    return true;
  }
  else
    return false;
}

