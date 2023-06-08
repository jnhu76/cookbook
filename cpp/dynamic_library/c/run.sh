#!/usr/bin/sh

rm libmymath.so main
gcc -fpic -shared algo.c -o libmymath.so
gcc main.c -ldl -o main