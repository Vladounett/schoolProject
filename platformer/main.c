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

    bool fini = false; bool collision = false;
    bool sauter = false; bool collisionSol = false;

    int tempSaut = 0;

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

    SDL_Rect p_rect; //déclaration du rectangle du joueur
    p_rect.x = 32*2;
    p_rect.y = (tailleY*32)-64;
    p_rect.w = 28;
    p_rect.h = 32;

    SDL_Rect sol_Collision;
    sol_Collision.x = p_rect.x;
    sol_Collision.y = p_rect.y+32;
    sol_Collision.h = 2;
    sol_Collision.w = 28;

    SDL_Rect overlap; //le rectangle qui stockera la collision entre deux rectangles

    SDL_Event events;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* fenetre = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tailleX*32, tailleY*32, SDL_WINDOW_BORDERLESS);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    ressources_t ressources; //structure ressources avec les textures et sprites nécessaires

    ressources.spriteJoueur = charger_image("ressources/character/Standing.bmp", rendu);
    ressources.sols[0] = charger_image("ressources/textures/sky.bmp", rendu);
    ressources.sols[1] = charger_image("ressources/textures/bricks.bmp", rendu);
    ressources.sols[2] = charger_image("ressources/textures/blueBricks.bmp", rendu);


    while(!fini){ //Boucle principal (on sort quand l'utilisateur appuie sur ECHAP ou clique sur la croix)

        keyStates = SDL_GetKeyboardState(NULL); //On récupére l'état actuel du clavier, et agit en conséquences (mouvement du personnage)

        if(keyStates[SDL_SCANCODE_A]){
            p_rect.x -= PLAYER_SPEED;
        } else if(keyStates[SDL_SCANCODE_D]){
            p_rect.x += PLAYER_SPEED;
        }
        if(keyStates[SDL_SCANCODE_SPACE] && sauter == false){
            sauter = true;
            tempSaut = 0;
        }

        if(sauter && tempSaut < 40){
            p_rect.y -= PLAYER_JUMP;
            tempSaut++;
        }else{
            p_rect.y += GRAVITY;
        }

        sol_Collision.x = p_rect.x;
        sol_Collision.y = p_rect.y+32;

        for(int i = 0; i < tailleX; i++){ //On regarde si le joueur est entré en collision avec le décor et on récupére la collision dans le rectangle "overlap"
            if(SDL_HasIntersection(&p_rect, &r_rect[i][0]) && currentLevel[i][0] != '0'){
                collision = true;
                SDL_IntersectRect(&p_rect, &r_rect[i][0], &overlap);
            }
            if(SDL_HasIntersection(&sol_Collision, &r_rect[i][0]) && currentLevel[i][0] != '0'){
                collisionSol = true;
            }
            for(int j = 0; j < tailleY; j++){
                if(SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] != '0'){
                    collision = true;
                    SDL_IntersectRect(&p_rect, &r_rect[i][0], &overlap);
                }
                if(SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] != '0'){
                    collisionSol = true;
                }
            }
        }


        if(collision && overlap.x > p_rect.x){ //Si le joueur est entré en collision on l'enlève de la dite collision
            p_rect.x -= PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
        }else if(collision && overlap.x <= p_rect.x){
            p_rect.x += PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
        }
        if(collisionSol){
            p_rect.y -= GRAVITY;
            collisionSol = false;
            sauter = false;
        }

        SDL_PollEvent(&events);

        switch(events.type){ //On réagit en fonction des événements
            case SDL_QUIT: //si on clique sur la croix on quitte
            fini = true; break;
            case SDL_KEYDOWN:
            switch(events.key.keysym.sym){
                case SDLK_ESCAPE: //si on appuie sur échap on quitte
                fini = true; break;
            }break;
        }

        SDL_RenderClear(rendu);

        for(int i = 0; i < tailleX; i++){  //Construction du niveau en fonction du tableau récupéré
            switch(currentLevel[i][0]){
                case '0':
                SDL_RenderCopy(rendu, ressources.sols[0], NULL, &r_rect[i][0]); break;
                case '1':
                SDL_RenderCopy(rendu, ressources.sols[1], NULL, &r_rect[i][0]); break;
                case '2':
                SDL_RenderCopy(rendu, ressources.sols[2], NULL, &r_rect[i][0]); break;
            }
            //printf("i = %d \n",i);
            for(int j = 0; j < tailleY; j++){
                switch(currentLevel[i][j]){
                    case '0':
                    SDL_RenderCopy(rendu, ressources.sols[0], NULL, &r_rect[i][j]); break;
                    case '1':
                    SDL_RenderCopy(rendu, ressources.sols[1], NULL, &r_rect[i][j]); break;
                    case '2':
                    SDL_RenderCopy(rendu, ressources.sols[2], NULL, &r_rect[i][j]); break;
                }
                //printf("i/j = %d/%d\n", i,j);
            }
        }

        SDL_RenderDrawRect(rendu, &sol_Collision);

        apply_texture(ressources.spriteJoueur, rendu, p_rect.x - 4, p_rect.y); //on applique la texture du joueur à la position du rectangle du joueur

        SDL_RenderPresent(rendu);

        SDL_Delay(10);
    }


    SDL_DestroyRenderer(rendu); //On quitte proprement
    SDL_DestroyWindow(fenetre); 
    SDL_Quit();

    return 0;
}