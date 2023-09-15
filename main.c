#include <stdio.h>
#include <unistd.h>

#include "terminal.h"

int main(void) {
    raw_mode();
    char c = '\0';
    while (1) {
        read(STDIN_FILENO, &c, 1);
        if (c == 'q') break;
    }
    return 0;
}
