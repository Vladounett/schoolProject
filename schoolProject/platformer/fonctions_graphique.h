#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

#ifndef FONCTIONS_SDL
#define FONCTIONS_SDL

SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
void apply_text(SDL_Renderer *renderer,int x, int y, int w, int h, const char *text);
void apply_texture(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y);
void apply_texture_flip(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y);
void apply_texture_via_rect(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y);
void apply_texture_via_rect_flip(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y);
void handle_animation(anim_t* a);
void init_anim(anim_t* a, int nbFrames, int tempsFrame);
void init_ressources(ressources_t* ressources, SDL_Renderer* rendu);

#endif
