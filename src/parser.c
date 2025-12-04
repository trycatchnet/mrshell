#include <string.h>
#include "parser.h"

int parse_args(char *buffer, char **args) {
   int argc = 0;
   char *token = strtok(buffer, " \t");

    /* \0 == NULL */

    while (token != NULL && argc < MAX_ARGS - 1) {
        args[argc++] = token;
        token = strtok(NULL, " \t");
    }

    args[argc] = NULL;
    return argc;
}
