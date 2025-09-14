#include "pipes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

// Array de pipes y args definido extern en .h
char *args[MAX_ARGS];
char *pipes[MAX_PIPES];

void split_args(char *entrada){
    int i = 0;
    args[i] = strtok(entrada, " ");
    while(args[i] != NULL && i < MAX_ARGS -1){
        args[++i] = strtok(NULL, " ");
    }
    args[i] = NULL;
}

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

void ejecutar_pipes(int MAX){
    int c_pipes[MAX][2];
    pid_t pid;

    for(int i = 0; i< MAX; i++){
        if(pipe(c_pipes[i]) == -1){
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < MAX;i++){
        pid = fork();
        if(pid == 0){
            if(i>0)
                dup2(c_pipes[i-1][0], STDIN_FILENO);
            if(i < MAX -1)
                dup2(c_pipes[i][1], STDOUT_FILENO);

            for(int j = 0; j < MAX -1; j++){
                close(c_pipes[j][0]);
                close(c_pipes[j][1]);
            }

            split_args(pipes[i]);
            execvp(args[0],args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < MAX - 1; i++){
        close(c_pipes[i][0]);
        close(c_pipes[i][1]);
    }

    for(int i = 0; i < MAX; i++)
        wait(NULL);
}

bool check(int *numero_pipes){
    *numero_pipes = 0;
    for(int i = 0; i<strlen(input); i++){
        if(input[i] == '|')
            (*numero_pipes)++;
    }
    return (*numero_pipes > 0);
}
