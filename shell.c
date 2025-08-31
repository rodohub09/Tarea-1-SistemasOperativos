#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_INPUT 1024
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;
    time_t ahora;

    while (1) {
      time(&ahora);
      struct tm *hora_local = localtime(&ahora);  // Convierte a hora local

      int hora = hora_local->tm_hour;
      int minutos = hora_local->tm_min;
      int segundos = hora_local->tm_sec;
      
      printf("%02d-%02d-%02d/shell/> ", hora,minutos,segundos); // Prompt
      fflush(stdout);
      
      if (fgets(input, MAX_INPUT, stdin) == NULL) {
	break;
      }

      // Se quita el salto de línea
      input[strcspn(input, "\n")] = '\0';
      
      // La shell se cierra al recibir exit
      if (strcmp(input, "exit") == 0) {
	break;
      }
      
      // Se separa el input en argumentos
      int i = 0;
      args[i] = strtok(input, " ");
      while (args[i] != NULL && i < MAX_ARGS - 1) {
	args[++i] = strtok(NULL, " ");
      }
      args[i] = NULL;
      
      // Se hace el fork para que el proceso hijo ejecute el comando
      pid = fork();
      
      if (pid == 0) {
	// Proceso hijo ejecuta el comando
	if (execvp(args[0], args) == -1) {
	  perror("Error ejecutando comando");
	}
	exit(EXIT_FAILURE);
      } else if (pid > 0) {
	// Proceso padre espera al hijo
	waitpid(pid, &status, 0);
      } else {
	perror("Error al realizar el fork");
      }
    }
    
    return 0;
}
