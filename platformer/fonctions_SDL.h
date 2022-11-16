#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#ifndef FONCTIONS_SDL
#define FONCTIONS_SDL

SDL_Texture* charger_image (const char* nomfichier, SDL_Renderer* renderer);
SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color); 

#endif
