#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include "editor.h"

void rawMode(void);
void disableRawMode(void);
void mainLoop(void);
void handleNormalMode(char *c);
void handleInsertMode(char *c);

typedef struct Terminal {
    struct termios _orig;
} Terminal;

Terminal term;

void mainLoop(void) {
    rawMode();
    while (1) {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        switch (getEditorMode(&editor)) {
        case E_NORMAL_MODE:
            if (c == 'q') return;
            handleNormalMode(&c);
            break;
        case E_INSERT_MODE:
            handleInsertMode(&c);
            break;
        }
    }
}

void rawMode(void) {
    if (tcgetattr(STDIN_FILENO, &term._orig) == -1) {
        perror("[tcgetattr]");
        exit(1);
    }
    atexit(disableRawMode);

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

void disableRawMode(void) {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &term._orig) == -1) {
        perror("[tcsetattr]");
        exit(1);
    }
}

void handleNormalMode(char *c) {
    switch (*c) {
    case 'i':
        updateEditorMode(&editor, E_INSERT_MODE);
        break;
    }
}

void handleInsertMode(char *c) {
    if (*c == 27) {
        updateEditorMode(&editor, E_NORMAL_MODE);
    } else {
        if (*c == 0x7F) {
            write(STDOUT_FILENO, "\b \b", 3);
        } else {
            write(STDOUT_FILENO, c, 1);
        }
    }
}
