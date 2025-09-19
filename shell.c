#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "pipes.h"
#include "varios.h"
#include <sys/times.h>

int main(){

  bool pipe;         
  int num_pipes = 0;
  int miprof;
  
  while(1){
    printf("Shell/> $ ");   // Prompt
    fflush(stdout);        // Vacia el buffer de salida

    //Espera una entrada 
    if (fgets(input, MAX_INPUT, stdin) == NULL) {
      break;
    }

    //Elimina el salto de linea en el input
    input[strcspn(input, "\n")] = '\0';
    if(strcmp(input, "exit") == 0){
      break;  
    }
    else if(strlen(input) == 0){  //En caso de solo presionar enter, pasa a la siguiente iteracion del while
      continue;
    }
    miprof = check_miprof();
    pipe = check(&num_pipes);     // Pipe llama a la funcion check y toma true o false, dependiendo de la existencia de pipes 
    
    if(!pipe){                   // En caso de no haber, se hace split de "input" para separar el comando de sus argumentos y luego se ejecuta
      split_args(input);
      ejecutar();
    }
    else{                       /* En caso de haber pipes, se hace split de los mismos para separar cada pipe y se llama la funcion ejecutar pipes
				   con num_pipes + 1 correspondiendo  a la cantidad de procesos necesarios */
      split_pipes();
      ejecutar_pipes(num_pipes + 1);
    }
    num_pipes = 0;              // Reiniciamos el valor para futuras ejecuciones de comandos con pipes
    ejec_miprof(miprof);
  }
  return 0;
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
  char *arg = strtok(input, " ");
  
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
      memmove(input,input+16,strlen(input+16)+1);
      return 2;
    }
    else{
      printf("Error al ingresar comando miprof\n");
      return 3;
    } 
  }
}
      
