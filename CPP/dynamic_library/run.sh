#!/usr/bin/sh

gcc -fpic -shared algo.c -o libmymath.so

gcc main.c -ldl -o main