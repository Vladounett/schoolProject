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

//applique une texture à une coordoonée x/y

void apply_texture(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;
    
    SDL_RenderCopy(renderer, texture, NULL, &dst);
}

//applique une texture à une coordoonée x/y, inversé horizontalement

void apply_texture_flip(SDL_Texture *texture,SDL_Renderer *renderer,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
    dst.x = x; dst.y=y;
    
    SDL_RenderCopyEx(renderer, texture, NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
}

//applique une partie d'une texture à une coordoonée x/y (utilisée pour l'animation de marche)

void apply_texture_via_rect(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    dst.h = s_rect.h; dst.w = s_rect.w;
    dst.x = x; dst.y=y;
    
    SDL_RenderCopy(renderer, texture, &s_rect, &dst);
}

//applique une partie d'une texture à une coordoonée x/y (utilisée pour l'animation de marche), inversé horizontalement

void apply_texture_via_rect_flip(SDL_Texture *texture,SDL_Renderer *renderer, SDL_Rect s_rect,int x, int y){
    SDL_Rect dst = {0, 0, 0, 0};
    
    dst.h = s_rect.h; dst.w = s_rect.w;
    dst.x = x; dst.y=y;
    
    SDL_RenderCopyEx(renderer, texture, &s_rect, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
}

//s'occupe d'incrémenter les images d'une animation pour l'animer

void handle_animation(anim_t* a){
    if(a->tempsEcoule >= a->tempsParFrame){

        a->frame++;
        a->tempsEcoule = 0;

        if(a->frame >= a->nbFrames){
            a->frame = 0;
        }
    }
}

//initialise une animation

void init_anim(anim_t* a, int nbFrames, int tempsFrame){
    a->frame = 0;
    a->tempsParFrame = tempsFrame;
    a->tempsEcoule = 0;
    a->nbFrames = nbFrames;  
}

//affiche un text sur l'écran

void apply_text(SDL_Renderer *renderer,int x, int y, int w, int h, const char *text){
	
    TTF_Font * font = TTF_OpenFont("ressources/font/font.ttf", 20);

    if(font == NULL){
        printf("%s \n", "FONT NULL !!");
    }

    SDL_Color color = {255, 255, 0, 255};
    
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
     
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dstrect2 = {x, y, w, h};

    SDL_RenderCopy(renderer, texture, NULL, &dstrect2);
    
    TTF_CloseFont(font);
}

//initialise les textures du jeu

void init_ressources(ressources_t* ressources, SDL_Renderer* rendu){
    ressources->spriteJoueur = charger_image("ressources/character/Standing.bmp", rendu);
    ressources->spritesJoueurMarche = charger_image("ressources/character/walk.bmp", rendu);
    ressources->spriteJoueurSaut = charger_image("ressources/character/Jumping.bmp", rendu);
    ressources->sols[0] = charger_image("ressources/textures/sky.bmp", rendu);
    ressources->sols[1] = charger_image("ressources/textures/bricks.bmp", rendu);
    ressources->sols[2] = charger_image("ressources/textures/lava.bmp", rendu);
    ressources->sols[3] = charger_image("ressources/textures/exit.bmp", rendu);
    ressources->sols[4] = charger_image("ressources/textures/lockedExit.bmp", rendu);
    ressources->sols[5] = charger_image("ressources/textures/spikes.bmp", rendu);
    ressources->cle = charger_image("ressources/textures/key.bmp", rendu);
}