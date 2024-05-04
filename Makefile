CC = gcc

CPPFLAGS = 
CFLAGS = -Wextra -Wall -Wshadow
LDFLAGS = $(shell sdl2-config --cflags --libs) -lm -lpng

CURR_FILES = $(wildcard *.c)
OTHER_FILES = ../pixel.c
FILES = $(CURR_FILES) $(OTHER_FILES)

CURR_OBJECTS = $(patsubst %.c, Build/%.o, $(CURR_FILES))
OTHER_OBJECTS = $(patsubst %.c, Build/%.o, $(notdir $(OTHER_FILES)))
OBJECTS = $(CURR_OBJECTS) $(OTHER_OBJECTS)

.PHONY: files run

files: 
	@echo $(CURR_FILES)
	@echo $(OTHER_FILES)
	@echo $(FILES)
	@echo ""
	@echo $(CURR_OBJECTS)
	@echo $(OTHER_OBJECTS)
	@echo $(OBJECTS)

Build/%.o: ../%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

Build/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

main: $(OBJECTS)
	$(CC) $(LDFLAGS) $(CFLAGS) $^ -o Build/main

run: main
	./Build/main $(ARGS)

debug: CFLAGS += -O0 -ggdb
debug: main
	@echo ${DARGS}
	gdb -ex "set debuginfod enabled off" -ex "set args $(DARGS)" -ex run ./Build/main
