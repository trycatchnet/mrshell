#ifndef UTILS_H
#define UTILS_H

#define HISTORY_FILE "mrshell.history"
#define MAX_HISTORY 256

#include <stddef.h>

#define MAX_INPUT_SIZE 516

void change_directory(char **args, int argc);
void get_history_path(char *path, size_t size);
void add_to_history(const char *command);
void show_history(void);

#endif
