CC = gcc
CFLAGS = -W -Wall -ansi -std=c99
LIBS = -L./SDL2_ttf/.libs
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf
INCLUDES = -I./SDL2_ttf
EXEC = main

all: $(EXEC)
main: fonctions_fichiers.o fonctions_SDL.o fonctions_Str.o main.o
	$(CC) -o main fonctions_fichiers.o fonctions_SDL.o fonctions_Str.o main.o $(LDFLAGS)
fonctions_fichiers.o: fonctions_fichiers.c
	$(CC) -o fonctions_fichiers.o -c fonctions_fichiers.c $(CFLAGS)
fonctions_SDL.o: fonctions_SDL.c
	$(CC) -o fonctions_SDL.o -c fonctions_SDL.c $(CFLAGS)
fonctions_Str.o: fonctions_Str.c
	$(CC) -o fonctions_Str.o -c fonctions_Str.c $(CFLAGS)
main.o: main.c fonctions_fichiers.h fonctions_SDL.h fonctions_Str.h
	$(CC) -o main.o -c main.c $(CFLAGS)
clean:
	rm -rf *.o *~
mrproper: clean
	rm -rf $(EXEC)