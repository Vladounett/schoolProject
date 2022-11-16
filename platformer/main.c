#include "fonctions_fichiers.h"
#include "fonctions_SDL.h"
#include "fonctions_Str.h"
#include "stdio.h"
#include "constante.h"
#include "stdlib.h"
#include "stdbool.h"
#include "struct.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>



int main(int argc, char *argv[]){

    int tailleX = 0; int tailleY = 0;

    char* path = "niveaux/"; //chemin vers le dossier
    char* completePath = easyStrcat(path, argv[1]); //chemin vers le fichier entré en argument
    char** currentLevel = lire_fichier(completePath); //lecture du niveau puis on le met dans un tableau

    const Uint8* keyStates;

    bool fini = false;

    taille_fichier(completePath, &tailleY, &tailleX); //récupération de la taille du fichier

    printf("tailleX/tailleY = %d/%d\n",tailleX,tailleY);

    SDL_Rect r_rect[tailleX][tailleY];

    for(int i = 0; i < tailleX; i++){ //Création d'une grille sur tout le rendu

        r_rect[i][0].x = 32 * i;
        r_rect[i][0].y = 0;
        r_rect[i][0].h = 32;
        r_rect[i][0].w = 32;

        for(int j = 0; j < tailleY; j++){

            r_rect[i][j].x = 32 * i;
            r_rect[i][j].y = 32 * j;
            r_rect[i][j].h = 32;
            r_rect[i][j].w = 32;

        }
    }

    sprite_t player; //déclaration du joueur
    player.x = 32*2;
    player.y = (tailleY*32)-64;
    player.w = 32;
    player.h = 32;

    SDL_Event events;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* fenetre = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tailleX*32, tailleY*32, SDL_WINDOW_BORDERLESS);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* spriteJoueur = charger_image("ressources/character/Standing.bmp", rendu);

    SDL_Texture* sols[3];

    sols[0] = charger_image("ressources/textures/sky.bmp", rendu);
    sols[1] = charger_image("ressources/textures/bricks.bmp", rendu);
    sols[2] = charger_image("ressources/textures/blueBricks.bmp", rendu);


    while(!fini){ //Boucle principal (on sort quand l'utilisateur appuie sur ECHAP ou clique sur la croix)

        keyStates = SDL_GetKeyboardState(NULL); //On récupére l'état actuel du clavier, et agit en conséquences (mouvement du personnage)

        if(keyStates[SDL_SCANCODE_A]){
            player.x -= PLAYER_SPEED;
        } else if(keyStates[SDL_SCANCODE_D]){
            player.x += PLAYER_SPEED;
        }

        SDL_PollEvent(&events);

        switch(events.type){ //On réagit en fonction des événements
            case SDL_QUIT:
            fini = true; break;
            case SDL_KEYDOWN:
            switch(events.key.keysym.sym){
                case SDLK_ESCAPE:
                fini = true; break;
            }break;
        }

        SDL_RenderClear(rendu);

        for(int i = 0; i < tailleX; i++){  //Construction du niveau en fonction du tableau récupéré
            switch(currentLevel[i][0]){
                case '0':
                SDL_RenderCopy(rendu, sols[0], NULL, &r_rect[i][0]); break;
                case '1':
                SDL_RenderCopy(rendu, sols[1], NULL, &r_rect[i][0]); break;
                case '2':
                SDL_RenderCopy(rendu, sols[2], NULL, &r_rect[i][0]); break;
            }
            //printf("i = %d \n",i);
            for(int j = 0; j < tailleY; j++){
                switch(currentLevel[i][j]){
                    case '0':
                    SDL_RenderCopy(rendu, sols[0], NULL, &r_rect[i][j]); break;
                    case '1':
                    SDL_RenderCopy(rendu, sols[1], NULL, &r_rect[i][j]); break;
                    case '2':
                    SDL_RenderCopy(rendu, sols[2], NULL, &r_rect[i][j]); break;
                }
                //printf("i/j = %d/%d\n", i,j);
            }
        }

        apply_texture(spriteJoueur, rendu, player.x, player.y);

        SDL_RenderPresent(rendu);

        SDL_Delay(10);
    }


    SDL_DestroyRenderer(rendu); //On quitte proprement
    SDL_DestroyWindow(fenetre); 
    SDL_Quit();

    return 0;
}