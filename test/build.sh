#!/bin/sh

gcc -I../include -c ../src/parser.c -o ../obj/parser.o -ggdb -DDEBUG
gcc -I../include -c ../src/lexer.c -o ../obj/lexer.o -ggdb -DDEBUG
gcc -I../include -c main.c -o ../obj/main.o -ggdb -DDEBUG
gcc ../obj/main.o ../obj/parser.o ../obj/lexer.o -o test
