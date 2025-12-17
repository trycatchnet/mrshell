#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>

/*
 * \033 -> ESC
 * [A -> Up
 * [B -> Down
 * [C -> Right
 * [D -> Left
 *
*/
static struct termios orig_termios;

void enable_raw_mode(void) {
    tcgetattr(STDIN_FILENO, &orig_termios); 
    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void disable_raw_mode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

static void redraw_line(const char *buffer, int cursor_pos, int buffer_len) {
    printf("\r\033[K"); 
    printf("mr_shell> %s", buffer);

    int diff = buffer_len - cursor_pos;
    if (diff > 0) {
        printf("\033[%dD", diff);
    }
    fflush(stdout);
}

static void insert_char(char *buffer, int *buffer_len, int *cursor_pos, char c) {
    if (*buffer_len >= MAX_INPUT_SIZE -1) return;

    for (int i = *buffer_len; i > *cursor_pos; i--) {
        buffer[i] = buffer [i - 1];
    }

    buffer[*cursor_pos] = c;
    (*buffer_len)++;
    (*cursor_pos)++;
    buffer[*buffer_len] = '\0';

    redraw_line(buffer, *cursor_pos, *buffer_len);
}

static void delete_char(char *buffer, int *buffer_len, int *cursor_pos) {
    if (*cursor_pos == 0) return;

    for (int i = *cursor_pos - 1; i < *buffer_len - 1; i++) {
        buffer[i] = buffer[i + 1];
    }

    (*buffer_len)--;
    (*cursor_pos)--;
    buffer[*buffer_len] = '\0';

    redraw_line(buffer, *cursor_pos, *buffer_len);
}

static void move_cursor_left(int *cursor_pos) {
    if (*cursor_pos > 0) {
        printf("\033[D");
        fflush(stdout);
        (*cursor_pos)--;
    }
}

static void move_cursor_right(int *cursor_pos, int buffer_len) {
    if (*cursor_pos < buffer_len) {
        printf("\033[C");
        fflush(stdout);
        (*cursor_pos)++;
    }
}

int read_line(char *buffer, size_t size) {
    int buffer_len = 0;
    int cursor_pos = 0;
    char c;

    buffer[0]  = '\0';

    enable_raw_mode();

    while (1) {
        if (read(STDIN_FILENO, &c, 1) != 1) break;

        if (c == '\n' || c == '\r') {
            printf("\n");
            break;
        } 

        if (c == 127 || c == 8) {
            delete_char(buffer, &buffer_len, &cursor_pos);
            continue;
        }

        if (c == '\033') {
            char seq[3];
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'D':
                        move_cursor_left(&cursor_pos);
                        break;
                    case 'C':
                        move_cursor_right(&cursor_pos, buffer_len);
                        break;
                }
            }
            continue;
        }

        /* CTRL +C */
        if (c == 3) {
            printf("^C\n");
            buffer[0] = '\0';
            buffer_len = 0;
            break;
        }

        if (isprint(c)) {
            insert_char(buffer, &buffer_len, &cursor_pos, c);
            continue;
        }
    }
    disable_raw_mode();
    return buffer_len;
}
