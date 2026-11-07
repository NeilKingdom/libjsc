#!/bin/sh

gcc -I../include -I../lib/ezc/include -c ../src/parser.c -o ../build/obj/parser.o -ggdb -DDEBUG
gcc -I../include -I../lib/ezc/include -c ../src/lexer.c -o ../build/obj/lexer.o -ggdb -DDEBUG
gcc -I../include -I../lib/ezc/include -c main.c -o ../build/obj/main.o -ggdb -DDEBUG
gcc ../build/obj/main.o ../build/obj/parser.o ../build/obj/lexer.o -o test
