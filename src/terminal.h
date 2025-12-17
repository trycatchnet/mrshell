#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>

#define MAX_INPUT_SIZE 516

void enable_raw_mode(void);
void disable_raw_mode(void);

int read_line(char *buffer, size_t size);

#endif
