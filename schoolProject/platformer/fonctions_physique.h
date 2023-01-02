#include <SDL2/SDL.h>
#include "stdbool.h"
#include "constante.h"

#ifndef FONCTIONS_PHYSIQUE
#define FONCTIONS_PHYSIQUE

void mouvementCollisionSol(SDL_Rect p_rect, SDL_Rect* s_rect);
void mouvements(const Uint8* keyStates, SDL_Rect* p_rect, int* sens, int* tempSaut, bool* saut);
//void handle_collisions(int tailleX, int tailleY, char** currentLevel, SDL_Rect* p_rect, SDL_Rect** r_rect, SDL_Rect sol_Collision, bool* collision, bool* collisionSol, bool* saut, bool* finNiveau);

#endif
