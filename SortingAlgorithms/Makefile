CC = gcc

CPPFLAGS = 
CFLAGS = -Wextra -Wall -Wshadow
# LDFLAGS = $(shell sdl2-config --cflags --libs) -lm -lpng
LDFLAGS = -lm -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lGLEW -lEGL -lGL -lGLU -lOpenGL

# CURR_FILES = $(wildcard *.c)
CURR_FILES = main.c  singlyLinkedList.c  vertex.c func.c
ALGO_FILES = ./Algorithms/bubbleSort.c
# ALGO_FILES = $(wildcard Algorithms/*.c)
# OTHER_FILES = ../pixel.c
FILES = $(CURR_FILES) $(ALGO_FILES) $(OTHER_FILES)

CURR_OBJECTS = $(patsubst %.c, Build/%.o, $(CURR_FILES))
ALGO_OBJECTS = $(patsubst Algorithms/%.c, Build/%.o, $(ALGO_FILES))
# OTHER_OBJECTS = $(patsubst %.c, Build/%.o, $(notdir $(OTHER_FILES)))
OBJECTS = $(CURR_OBJECTS) $(ALGO_OBJECTS) $(OTHER_OBJECTS)

.PHONY: files run

files: 
	@echo $(CURR_FILES)
	@echo $(ALGO_FILES)
	@echo $(OTHER_FILES)
	@echo $(FILES)
	@echo ""
	@echo $(CURR_OBJECTS)
	@echo $(ALGO_OBJECTS)
	@echo $(OTHER_OBJECTS)
	@echo $(OBJECTS)

Build/%.o: ../%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

Build/%.o: Algorithms/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

Build/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

main: $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o Build/main

run: main
	./Build/main $(ARGS)

release: CFLAGS += -O3
release: main

debug: CFLAGS += -O0 -ggdb
debug: main
	@echo ${DARGS}
	gdb -ex "set debuginfod enabled off" -ex "set args $(DARGS)" -ex run ./Build/main
