#include <SDL2/SDL.h>

#ifndef STRUCT
#define STRUCT

struct ressources_s{
    SDL_Texture* spriteJoueur;
    SDL_Texture* sols[3];
};
typedef struct ressources_s ressources_t;

#endif