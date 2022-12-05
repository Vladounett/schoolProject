CC = gcc
CFLAGS = -W -Wall -ansi -std=c99
LIBS = -L./SDL2_ttf/.libs
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf
INCLUDES = -I./SDL2_ttf
EXEC = main

all: $(EXEC)
main: fonctions_fichiers.o fonctions_graphique.o fonctions_Str.o fonctions_physique.o main.o
	$(CC) -o main fonctions_fichiers.o fonctions_graphique.o fonctions_Str.o fonctions_physique.o main.o $(LDFLAGS)
fonctions_fichiers.o: fonctions_fichiers.c
	$(CC) -o fonctions_fichiers.o -c fonctions_fichiers.c $(CFLAGS)
fonctions_graphique.o: fonctions_graphique.c
	$(CC) -o fonctions_graphique.o -c fonctions_graphique.c $(CFLAGS)
fonctions_Str.o: fonctions_Str.c
	$(CC) -o fonctions_Str.o -c fonctions_Str.c $(CFLAGS)
fonctions_physique.o: fonctions_physique.c
	$(CC) -o fonctions_physique.o -c fonctions_physique.c $(CFLAGS)
main.o: main.c fonctions_fichiers.h fonctions_graphique.h fonctions_Str.h fonctions_physique.h
	$(CC) -o main.o -c main.c $(CFLAGS)
clean:
	rm -rf *.o *~
mrproper: clean
	rm -rf $(EXEC)