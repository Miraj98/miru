#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>

#include "editor.h"

// Terminal escape sequences
#define CLEAR_SCREEN "\x1b[2J"
#define CURSOR_START "\x1b[H"
// Escape sequences

#define MAX_STDOUT_BUF 1024

typedef struct Terminal {
    struct termios _orig;
    char stdout_buf[MAX_STDOUT_BUF];
    int stdout_buf_len;
} Terminal;

Terminal term;

void rawMode(void);
void disableRawMode(void);
void mainLoop(void);
void handleNormalMode(char *c);
void handleInsertMode(char *c);
void clearScreen(void);
void repositionCursor(int row, int col);
void drawRows(void);
void appendBuf(Terminal *t, char *c, int len);
void flushBufToStdout(Terminal *t);


void appendBuf(Terminal *t, char *c, int len) {
    if (t->stdout_buf_len + len > MAX_STDOUT_BUF) {
        fprintf(stderr, "Max buffer size reached for Terminal.stdout_buf\n");
        exit(1);
    }
    int i;
    for (i = 0; i < len; ++i) {
        t->stdout_buf[t->stdout_buf_len + i] = c[i];
    }
    t->stdout_buf_len += len;
}

void flushBufToStdout(Terminal *t) {
    if (t->stdout_buf_len > 0) {
        write(STDOUT_FILENO, t->stdout_buf, t->stdout_buf_len);
        t->stdout_buf_len = 0;
    }
}

void mainLoop(void) {
    rawMode();
    clearScreen();
    drawRows();
    flushBufToStdout(&term);
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
        flushBufToStdout(&term);
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
    clearScreen();
    flushBufToStdout(&term);
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
            appendBuf(&term, "\b \b", 3);
        } else {
            appendBuf(&term, c, 1);
        }
    }
}

void clearScreen(void) {
    // Clear screen and reposition cursor to start
    appendBuf(&term, "\x1b[2J\x1b[H", 7);
}

void drawRows(void) {
    struct winsize ws;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        perror("[getWindowSize]");
        exit(1);
    }
    setEditorSize(&editor, ws.ws_row, ws.ws_col);
    int r;
    for (r = 0; r < getEditorRows(&editor); r++) {
        if (r == getEditorRows(&editor) - 1) {
            appendBuf(&term, "--- NORMAL ---", 14);
        } else {
            appendBuf(&term, "~\r\n", 3);
        }
    }

    appendBuf(&term, "\x1b[1;2H", 6);
}

