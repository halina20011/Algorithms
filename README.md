# SDL2
Repo of different algorithms / games implemented in C with SDL2 library </br>

## List
Marching squares </br>
![Preview](/Images/marchingSquares.png) </br>
Pong </br>
![Preview](/Images/pong.png) </br>
Mandelbrot set </br>
![Preview](/Images/mandelbrotSet.png) </br>

## SDL2:
`yay -S sdl2` </br>
`yay -S sdl2_image` </br>

## Setup
`cd [name]` </br>
`mkdir Build` </br>

## Build and run
`gcc main.c $(sdl2-config --cflags --libs) -o Build/main && ./Build/main` </br>
