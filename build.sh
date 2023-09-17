#!/bin/sh

gcc -g -c main.c terminal.c editor.c -Wall -Wextra -pedantic
gcc -g main.o terminal.o editor.o -o miru -Wall -Wextra -pedantic
rm *.o
