#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void change_directory(char **args, int argc) {
   if (argc < 2) {
       char *home = getenv("HOME");
       if (home != NULL) {
            if (chdir(home) != 0) {
                perror("mr_shell> cd");
            }
       }
   } else {
        if (chdir(args[1]) != 0) {
            perror("mr_shell> cd");
        }
   }  
}
