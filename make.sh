#!/bin/sh
SOURCE="pswdfind.c"
EXEC="pswdfind"
gcc $SOURCE -o $EXEC -g -lm -fopenmp -O3
