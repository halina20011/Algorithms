#!/bin/sh

mkdir -p ./Build
# GCCFLAGS="-Wextra -Wall -Wshadow"
GCCFLAGS=""
LIBS="$(sdl2-config --cflags --libs) -lm -lpng"
gcc main.c $LIBS $GCCFLAGS -o Build/main
