cc = clang

cflags = -Wall -Werror -pedantic -std=c99 -O2
dflags = -Wall -Werror -pedantic -std=c99 -g

sdl_include = $(shell pkg-config --cflags sdl3)
sdl_lib = $(shell pkg-config --libs sdl3)

# Este es solo para buscar los archivos .c en src/
#source = $(wildcard src/*.c)

# Esto hace que el Makefile busque archivos .c en src/ y en todas sus subcarpetas
source = $(wildcard src/**/*.c) $(wildcard src/*.c)

DVAR=

output = bin/render_cpu

build:
	mkdir -p bin
	$(cc) $(cflags) $(dflags) $(DVAR) $(source) -o $(output) $(sdl_include) $(sdl_lib) -lm

clean:
	rm -rf bin

run:
	./$(output)

