#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

void raw_mode(void);
void disable_raw_mode(void);

typedef struct Terminal {
    struct termios _orig;
} Terminal;

Terminal term;


void raw_mode(void) {
    if (tcgetattr(STDIN_FILENO, &term._orig) == -1) {
        perror("[tcgetattr]");
        exit(1);
    }
    atexit(disable_raw_mode);

    struct termios raw = term._orig;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("[tcsetattr]");
        exit(1);
    }
}

void disable_raw_mode(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term._orig) == -1) {
        perror("[tcsetattr]");
        exit(1);
    }
}
