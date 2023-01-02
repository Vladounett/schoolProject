#include <SDL2/SDL.h>

#ifndef STRUCT
#define STRUCT

//struct d'une animation

struct anim_s{
    int frame;
    int tempsParFrame;
    int tempsEcoule;
    int nbFrames;
};
typedef struct anim_s anim_t;


//struct des ressources du jeu

struct ressources_s{
    SDL_Texture* spriteJoueur;
    SDL_Texture* spritesJoueurMarche;
    SDL_Texture* spriteJoueurSaut;
    SDL_Texture* cle;
    SDL_Texture* sols[5];
};
typedef struct ressources_s ressources_t;

#endif