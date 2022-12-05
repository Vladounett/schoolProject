#include "fonctions_physique.h"

void mouvementCollisionSol(SDL_Rect* p_rect, SDL_Rect* s_rect){
    s_rect->x = p_rect->x+4;
    s_rect->y = p_rect->y+30;
}