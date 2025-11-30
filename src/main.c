#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* 512 byte input
 * 1 byte \n
 * 1 byte \0
 * 2 byte safety (güvenlik)
 * */
#define MAX_INPUT_SIZE 516

int main(void) {
   
    char *args[2];
    char buffer[MAX_INPUT_SIZE];
    int status;
    printf("mr_shell> _");
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

    if (strcmp(buffer, "exit") == 0) {
       exit(EXIT_SUCCESS); 
    }

    pid_t pid = fork();

    if (pid == 0) {
       args[0] = buffer;
       args[1] = NULL;
       execvp(args[0], args);
    
       perror("The command can't be runned");
       exit(EXIT_FAILURE);
    } else if (pid > 0) { 
        waitpid(pid, &status, 0); 
    } else {
        perror("Fork error");
    }
}
