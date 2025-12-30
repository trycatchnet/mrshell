#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

void get_history_path(char *path, size_t size) {
    char *home = getenv("HOME");

    if (home != NULL) {
        snprintf(path, size, "%s/%s", home, HISTORY_FILE);
    } else {
        snprintf(path, size, "%s", HISTORY_FILE);        
    }
}

static int is_empty_or_whitespace(const char *str) {
    if (str == NULL || str[0] == '\0') return 1;

    while (*str) {
        if (!isspace((unsigned char)*str)) return 0;
        str++;
    }
    return 1;
}

void add_to_history(const char *command) {
    if (is_empty_or_whitespace(command)) return;

    char history_path[512];
    get_history_path(history_path, sizeof(history_path));
    
    char *lines[MAX_HISTORY];
    int line_count = 0;

    FILE *f = fopen(history_path, "r");

    if (f != NULL) {
        char line[MAX_INPUT_SIZE];
        while (fgets(line, sizeof(line), f) && line_count < MAX_HISTORY) {
           line[strcspn(line, "\n")] = '\0';

           if (!is_empty_or_whitespace(line)) {
            lines[line_count] = strdup(line);
            line_count++;
           }
        }
        fclose(f);
    }

    if (line_count >= MAX_HISTORY) {
        free(lines[0]);
        for (int i = 0; i < line_count - 1; i++) {
            lines[i] = lines[i + 1];
        }
        line_count--;
    }

    lines[line_count] = strdup(command);
    line_count++;

    f = fopen(history_path, "w");
    if (f != NULL) {
        for (int i = 0; i < line_count; i++) {
            fprintf(f, "%s\n", lines[i]);
            free(lines[i]);
        }
        fclose(f);
    } else {
        /* memory cleaning... */
        for (int i = 0; i < line_count; i++) {
            free(lines[i]);
        }
    }
}

void show_history(void) {
    char history_path[512];
    get_history_path(history_path, sizeof(history_path));

    FILE *f = fopen(history_path, "r");
    if (f == NULL) {
        printf("mr_shell> History not found.\n");
        return;
    }

    char line[MAX_INPUT_SIZE];
    int line_num = 1;

    while (fgets(line, sizeof(line), f)) {    
        if (!is_empty_or_whitespace(line)) {
            printf("%4d  %s", line_num++, line); 
        }
    }

    fclose(f);
}
