#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "parser.h"

/* 512 byte input
 * 1 byte \n
 * 1 byte \0
 * 2 byte safety (güvenlik)
 * */
#define MAX_INPUT_SIZE 516

int main(void) {
   
    char *args[MAX_ARGS];
    char buffer[MAX_INPUT_SIZE];
    int status;

    while (1) {

    printf("mr_shell> ");
    fflush(stdout); /* It's important to use fflush(stdout) we are flushing the output before getting a input.*/
    fgets(buffer, sizeof(buffer), stdin);

    /*
     * Example algorithm that you can use.
     *
    for (int i = 0; i < MAX_INPUT_SIZE; i++) {
        if (buffer[i] == '\n') buffer[i] = '\0';
    }
    */

    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0') continue;

    if (strcmp(buffer, "exit;") == 0) {
       exit(EXIT_SUCCESS); 
    }

    int argc = parse_args(buffer, args);

    if (argc == 0) continue; 

    pid_t pid = fork();

    if (pid == 0) {
       execvp(args[0], args); 
       perror("The command can't be runned");
       exit(EXIT_FAILURE);
    } else if (pid > 0) { 
        waitpid(pid, &status, 0); 
    } else {
        perror("Fork error");
    }

    }
}
