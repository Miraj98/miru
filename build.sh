#!/bin/sh

gcc -c main.c terminal.c editor.c -Wall -Wextra -pedantic
gcc  main.o terminal.o editor.o -o miru -Wall -Wextra -pedantic
rm *.o
