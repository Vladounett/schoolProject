#include "fonctions_graphique.h"

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

void apply_texture_flip(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;
    
    SDL_RenderCopyEx(renderer, texture, NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void apply_texture_via_rect(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    dst.h = s_rect.h; dst.w = s_rect.w;
    dst.x = x; dst.y=y;
    
    SDL_RenderCopy(renderer, texture, &s_rect, &dst);
}

void apply_texture_via_rect_flip(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    dst.h = s_rect.h; dst.w = s_rect.w;
    dst.x = x; dst.y=y;
    
    //SDL_RenderCopy(renderer, texture, &s_rect, &dst);
    SDL_RenderCopyEx(renderer, texture, &s_rect, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
}

void handle_animation(anim_t* a){
    if(a->tempsEcoule >= a->tempsParFrame){

        a->frame++;
        a->tempsEcoule = 0;

        if(a->frame >= a->nbFrames){
            a->frame = 0;
        }
    }
}