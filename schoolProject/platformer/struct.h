#include <SDL2/SDL.h>

#ifndef STRUCT
#define STRUCT

struct anim_s{
    int frame;
    int tempsParFrame;
    int tempsEcoule;
    int nbFrames;
};
typedef struct anim_s anim_t;

struct ressources_s{
    SDL_Texture* spriteJoueur;
    SDL_Texture* spritesJoueurMarche;
    SDL_Texture* spriteJoueurSaut;
    SDL_Texture* cle;
    SDL_Texture* sols[5];
};
typedef struct ressources_s ressources_t;



typedef struct Liste *liste; 
struct Liste {
  int premier ; 
  liste suivant ;
};


#endif