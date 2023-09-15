#!/bin/sh

gcc -c main.c terminal.c -Wall -Wextra -pedantic
gcc  main.o terminal.o -o miru -Wall -Wextra -pedantic
rm *.o
