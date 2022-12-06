#include "fonctions_fichiers.h"
#include "fonctions_graphique.h"
#include "fonctions_Str.h"
#include "fonctions_physique.h"
#include "stdio.h"
#include "constante.h"
#include "stdlib.h"
#include "stdbool.h"
#include "struct.h"
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>



int main(void){

    int tailleX = 0; int tailleY = 0;

    int currentLevelName = 0;
    
    char* levelNames[2];
    levelNames[0] = "niveau1.txt";
    levelNames[1] = "niveau2.txt";

    char* path = "niveaux/"; //chemin vers le dossier
    char* completePath = easyStrcat(path, levelNames[currentLevelName]); //chemin vers le fichier du niveau
    char** currentLevel = lire_fichier(completePath); //lecture du niveau puis on le met dans un tableau

    const Uint8* keyStates;

    bool fini = false; bool collision = false;
    bool saut = false; bool collisionSol = false;
    bool finNiveau = false;

    int tempSaut = 0; int niveauCollision = 0;
    int sens = 0; int sensIdle = 1;

    taille_fichier(completePath, &tailleY, &tailleX); //récupération de la taille du fichier

    anim_t anim_joueur; //Déclaration de l'animation du joueur
    anim_joueur.frame = 0;
    anim_joueur.tempsParFrame = 10;
    anim_joueur.tempsEcoule = 0;
    anim_joueur.nbFrames = 5;

    printf("%s \n",completePath);
    printf("tailleX/tailleY = %d/%d\n",tailleX,tailleY);

    SDL_Rect r_rect[tailleX][tailleY];

    for(int i = 0; i < tailleX; i++){ //Création d'une grille sur tout le rendu
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
    p_rect.w = 24;
    p_rect.h = 26;

    SDL_Rect sol_Collision; //déclaration du rectangle qui sert à détecter le sol
    sol_Collision.x = p_rect.x+4;
    sol_Collision.y = p_rect.y+32;
    sol_Collision.h = 1;
    sol_Collision.w = 17;

    SDL_Rect joueurMarche[5]; //Rectangle de l'animation de marche du joueur

    for(int i = 0; i < 6; i++){
        joueurMarche[i].x = i*32;
        joueurMarche[i].y = 0;
        joueurMarche[i].h = 32;
        joueurMarche[i].w = 32; 
    }

    SDL_Rect overlap; //le rectangle qui stockera la collision entre deux rectangles

    SDL_Event events;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* fenetre = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tailleX*32, tailleY*32, SDL_WINDOW_BORDERLESS);
    SDL_Renderer* rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    ressources_t ressources; //structure ressources avec les textures et sprites nécessaires

    ressources.spriteJoueur = charger_image("ressources/character/Standing.bmp", rendu);
    ressources.spritesJoueurMarche = charger_image("ressources/character/walk.bmp", rendu);
    ressources.sols[0] = charger_image("ressources/textures/sky.bmp", rendu);
    ressources.sols[1] = charger_image("ressources/textures/bricks.bmp", rendu);
    ressources.sols[2] = charger_image("ressources/textures/blueBricks.bmp", rendu);
    ressources.sols[3] = charger_image("ressources/textures/exit.bmp", rendu);


    while(!fini){ //Boucle principal (on sort quand l'utilisateur appuie sur ECHAP ou clique sur la croix)

        //On regarde les touches pressées

        keyStates = SDL_GetKeyboardState(NULL); //On récupére l'état actuel du clavier, et agit en conséquences (mouvement du personnage)

        if(keyStates[SDL_SCANCODE_A]){
            p_rect.x -= PLAYER_SPEED;
            sens = -1;
        } else if(keyStates[SDL_SCANCODE_D]){
            p_rect.x += PLAYER_SPEED;
            sens = 1;
        } else {
            sens = 0;
        }
        if(keyStates[SDL_SCANCODE_SPACE] && !saut){
            tempSaut = 0;
            saut = true;
        }

        if(saut && tempSaut < PLAYER_JUMP_TIME){
            p_rect.y -= PLAYER_JUMP;
            tempSaut++;
        }else{
            p_rect.y += GRAVITY;
        }

        mouvementCollisionSol(&p_rect, &sol_Collision); //On bouge le rectangle qui détecte la collision avec le sol, pour le mettre sous le joueur

        //Collisions :

        for(int i = 0; i < tailleX; i++){ //On regarde si le joueur est entré en collision avec le décor et on récupére la collision dans le rectangle "overlap"
            for(int j = 0; j < tailleY; j++){
                if(SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '1'){
                    collision = true;
                    SDL_IntersectRect(&p_rect, &r_rect[i][0], &overlap);
                }
                if(SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '1'){
                    collisionSol = true;
                    niveauCollision = (j*32)-32;
                }
                if(SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '3'){
                    finNiveau = true;
                }                
                if(SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '3'){
                    finNiveau = true;
                }
            }
        }

        if(collisionSol){
            p_rect.y = niveauCollision;
            collisionSol = false;
            saut = false;
            mouvementCollisionSol(&p_rect, &sol_Collision);
        }

        if(collision && overlap.x > p_rect.x){ //Si le joueur est entré en collision on l'enlève de la dite collision
            p_rect.x -= PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
            mouvementCollisionSol(&p_rect, &sol_Collision);
        }else if(collision && overlap.x <= p_rect.x){
            p_rect.x += PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
            mouvementCollisionSol(&p_rect, &sol_Collision);
        }

        //Rendu

        SDL_RenderClear(rendu);

        for(int i = 0; i < tailleX; i++){  //Construction du niveau en fonction du tableau récupéré
            for(int j = 0; j < tailleY; j++){
                switch(currentLevel[i][j]){
                    case '0':
                    SDL_RenderCopy(rendu, ressources.sols[0], NULL, &r_rect[i][j]); break;
                    case '1':
                    SDL_RenderCopy(rendu, ressources.sols[1], NULL, &r_rect[i][j]); break;
                    case '2':
                    SDL_RenderCopy(rendu, ressources.sols[2], NULL, &r_rect[i][j]); break;
                    case '3':
                    SDL_RenderCopy(rendu, ressources.sols[3], NULL, &r_rect[i][j]); break;
                }
                //printf("i/j = %d/%d\n", i,j);
            }
        }



        switch(sens){ //on applique la texture du joueur à la position du rectangle du joueur
            case 0:
                anim_joueur.tempsEcoule = 0;
                switch(sensIdle){
                    case 1:
                        apply_texture(ressources.spriteJoueur, rendu, p_rect.x - 4, p_rect.y); break;
                    case -1:
                        apply_texture_flip(ressources.spriteJoueur, rendu, p_rect.x - 4, p_rect.y); break;
                }
            break;
            case 1:
                apply_texture_via_rect(ressources.spritesJoueurMarche, rendu, joueurMarche[anim_joueur.frame], p_rect.x - 4, p_rect.y);
                anim_joueur.tempsEcoule++;
                sensIdle = 1;
                break;
            case -1:
                apply_texture_via_rect_flip(ressources.spritesJoueurMarche, rendu, joueurMarche[anim_joueur.frame], p_rect.x - 4, p_rect.y);
                anim_joueur.tempsEcoule++;
                sensIdle = -1;
                break;
        }

        handle_animation(&anim_joueur); //On s'occupe de l'animation du joueur

        //SDL_RenderDrawRect(rendu, &sol_Collision);
        //SDL_RenderDrawRect(rendu, &p_rect);
        //SDL_RenderDrawRect(rendu, &overlapSol);

        SDL_RenderPresent(rendu);

        //Evenements

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

        if(finNiveau){

            currentLevelName++;

            completePath = easyStrcat(path, levelNames[currentLevelName]); //chemin vers le fichier du niveau
            printf("%s \n", completePath);
            currentLevel = lire_fichier(completePath); //lecture du niveau puis on le met dans un tableau

            p_rect.x = 32*2;
            p_rect.y = (tailleY*32)-64;

            finNiveau = false;
        }

        SDL_Delay(10);
    }


    SDL_DestroyRenderer(rendu); //On quitte proprement
    SDL_DestroyWindow(fenetre); 
    SDL_Quit();

    return 0;
}