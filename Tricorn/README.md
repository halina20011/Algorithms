# Tricorn

## SDL2
`yay -S sdl2` </br>
`yay -S sdl2_image` </br>

## Setup
`mkdir Build` </br>

## Build and run
`gcc main.c $(sdl2-config --cflags --libs) -lm -lpng -o Build/main && ./Build/main` </br>
