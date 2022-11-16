#include "fonctions_SDL.h"

//Fonction qui retourne une texture contenant un message

SDL_Texture* charger_texte(const char* message, SDL_Renderer* renderer, TTF_Font *font, SDL_Color color){
    SDL_Surface* surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

//Fonction qui retourne une texture depuis une image

SDL_Texture* charger_image(const char* nomfichier, SDL_Renderer* renderer){

    SDL_Surface* surface = SDL_LoadBMP(nomfichier);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}

void apply_texture(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}
