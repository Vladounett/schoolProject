#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "struct.h"

#ifndef FONCTIONS_SDL
#define FONCTIONS_SDL

SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color);
void apply_texture(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y);
void apply_texture_flip(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y);
void apply_texture_via_rect(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y);
void apply_texture_via_rect_flip(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y);
void handle_animation(anim_t* a);

#endif
