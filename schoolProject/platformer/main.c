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

int main(void)
{
    int tailleX = 0;
    int tailleY = 0;

    int currentLevelName = 0;

    char *levelNames[2];
    levelNames[0] = "niveau1.txt";
    levelNames[1] = "niveau2.txt";

    char *path = "niveaux/";                                             // chemin vers le dossier
    char *completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
    char **currentLevel = lire_fichier(completePath);                    // lecture du niveau puis on le met dans un tableau

    taille_fichier(completePath, &tailleY, &tailleX); // récupération de la taille du fichier

    int nbPickups = 0;
    SDL_Rect* pickups = malloc(sizeof(SDL_Rect));
    currentLevel = modifier_caractere(currentLevel, pickups, &nbPickups, tailleX, tailleY, '4', ' ');

    const Uint8 *keyStates;

    bool fini = false;
    bool collision = false;
    bool saut = false;
    bool collisionSol = false;
    bool finNiveau = false;

    int tempSaut = 0;
    int sens = 0;
    int sensIdle = 1;

    anim_t anim_joueur; // Déclaration de l'animation du joueur
    init_anim(&anim_joueur, 5, 10);

    printf("%s \n", completePath);
    printf("tailleX/tailleY = %d/%d\n", tailleX, tailleY);

    SDL_Rect **r_rect = (SDL_Rect **)malloc(sizeof(SDL_Rect *) * tailleX); //allocation mémoire de la grille du rendu
    for (int i = 0; i < tailleX; i++)
    {
        r_rect[i] = (SDL_Rect *)malloc(sizeof(SDL_Rect) * tailleY);
    }

    for (int i = 0; i < tailleX; i++)
    { // Création d'une grille sur tout le rendu
        for (int j = 0; j < tailleY; j++)
        {

            r_rect[i][j].x = 32 * i;
            r_rect[i][j].y = 32 * j;
            r_rect[i][j].h = 32;
            r_rect[i][j].w = 32;
        }
    }

    int niveauCollision = 0;
    SDL_Rect overlap; //déclaration du rectangle qui enregistrera les collisions entre le joueur et le reste

    SDL_Rect p_rect; // déclaration du rectangle du joueur
    p_rect.x = 32 * 2;
    p_rect.y = (tailleY * 32) - 64;
    p_rect.w = 24;
    p_rect.h = 26;

    SDL_Rect sol_Collision; // déclaration du rectangle qui sert à détecter le sol
    sol_Collision.x = p_rect.x + 4;
    sol_Collision.y = p_rect.y + 32;
    sol_Collision.h = 1;
    sol_Collision.w = 17;

    SDL_Rect joueurMarche[5]; // Rectangles de l'animation de marche du joueur

    for (int i = 0; i < 6; i++)
    {
        joueurMarche[i].x = i * 32;
        joueurMarche[i].y = 0;
        joueurMarche[i].h = 32;
        joueurMarche[i].w = 32;
    }

    SDL_Event events;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *fenetre = SDL_CreateWindow("main", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, tailleX * 32, tailleY * 32, SDL_WINDOW_BORDERLESS);
    SDL_Renderer *rendu = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    ressources_t *ressources = malloc(sizeof(ressources_t)); // structure ressources avec les textures et sprites nécessaires
    init_ressources(ressources, rendu);

    while (!fini)
    { // Boucle principal (on sort quand l'utilisateur appuie sur ECHAP ou clique sur la croix)

        keyStates = SDL_GetKeyboardState(NULL);

        mouvements(keyStates, &p_rect, &sens, &tempSaut, &saut); //On bouge le personnage en fonction des touches pressées

        mouvementCollisionSol(p_rect, &sol_Collision); // On bouge le rectangle qui détecte la collision avec le sol, pour le mettre sous le joueur

        //Collision

        for (int i = 0; i < tailleX; i++)
        { // On regarde si le joueur est entré en collision avec le décor et on récupére la collision dans le rectangle "overlap"
            for (int j = 0; j < tailleY; j++)
            {
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '1')
                {
                    collision = true;
                    SDL_IntersectRect(&p_rect, &r_rect[i][0], &overlap);
                }
                if (SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '1')
                {
                    collisionSol = true;
                    niveauCollision = (j * 32) - 32;
                }
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '3')
                {
                    finNiveau = true;
                }
                if (SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '3')
                {
                    finNiveau = true;
                }
            }
        }

        for(int i = 0; i < nbPickups; i++){
            if(SDL_HasIntersection(&p_rect, &pickups[i])){
                printf("collision clé n°%d \n", i);
            }
        }

        if (collisionSol)
        {
            p_rect.y = niveauCollision;
            collisionSol = false;
            saut = false;
            mouvementCollisionSol(p_rect, &sol_Collision);
        }

        if (collision && overlap.x > p_rect.x)
        { // Si le joueur est entré en collision on l'enlève de la dite collision
            p_rect.x -= PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
            mouvementCollisionSol(p_rect, &sol_Collision);
        }
        else if (collision && overlap.x <= p_rect.x)
        {
            p_rect.x += PLAYER_SPEED;
            collision = false;
            overlap.w = 0;
            overlap.h = 0;
            mouvementCollisionSol(p_rect, &sol_Collision);
        }

        // Rendu

        SDL_RenderClear(rendu);

        for (int i = 0; i < tailleX; i++)
        { // Construction du niveau en fonction du tableau récupéré
            for (int j = 0; j < tailleY; j++)
            {
                switch (currentLevel[i][j])
                {
                case '1':
                    SDL_RenderCopy(rendu, ressources->sols[1], NULL, &r_rect[i][j]);
                    break;
                case '2':
                    SDL_RenderCopy(rendu, ressources->sols[2], NULL, &r_rect[i][j]);
                    break;
                case '3':
                    SDL_RenderCopy(rendu, ressources->sols[3], NULL, &r_rect[i][j]);
                    break;
                default:
                    SDL_RenderCopy(rendu, ressources->sols[0], NULL, &r_rect[i][j]);
                }
                // printf("i/j = %d/%d\n", i,j);
            }
        }

        for(int i = 0; i < nbPickups; i++){
            SDL_RenderCopy(rendu, ressources->cle, NULL, &pickups[i]);
        }

        switch (sens)
        { // on applique la texture du joueur à la position du rectangle du joueur
        case 0:
            anim_joueur.tempsEcoule = 0;
            switch (sensIdle)
            {
            case 1:
                apply_texture(ressources->spriteJoueur, rendu, p_rect.x - 4, p_rect.y);
                break;
            case -1:
                apply_texture_flip(ressources->spriteJoueur, rendu, p_rect.x - 4, p_rect.y);
                break;
            }
            break;
        case 1:
            if (!saut)
            {
                apply_texture_via_rect(ressources->spritesJoueurMarche, rendu, joueurMarche[anim_joueur.frame], p_rect.x - 4, p_rect.y);
                anim_joueur.tempsEcoule++;
                sensIdle = 1;
                break;
            }
            else
            {
                apply_texture(ressources->spriteJoueurSaut, rendu, p_rect.x - 4, p_rect.y);
                break;
            }
        case -1:
            if (!saut)
            {
                apply_texture_via_rect_flip(ressources->spritesJoueurMarche, rendu, joueurMarche[anim_joueur.frame], p_rect.x - 4, p_rect.y);
                anim_joueur.tempsEcoule++;
                sensIdle = -1;
                break;
            }
            else
            {
                apply_texture_flip(ressources->spriteJoueurSaut, rendu, p_rect.x - 4, p_rect.y);
                break;
            }
        }

        handle_animation(&anim_joueur); // On s'occupe de l'animation du joueur

        if (finNiveau)
        {

            currentLevelName++;

            completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
            printf("%s \n", completePath);
            currentLevel = lire_fichier(completePath); // lecture du niveau puis on le met dans un tableau

            p_rect.x = 32 * 2;
            p_rect.y = (tailleY * 32) - 64;

            finNiveau = false;

            apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 32, 200, 80, "NEXT LEVEL");
            SDL_RenderPresent(rendu);

            SDL_Delay(2000);
        }

        //SDL_RenderDrawRect(rendu, &sol_Collision);

        SDL_RenderPresent(rendu);

        // Evenements

        SDL_PollEvent(&events);

        switch (events.type)
        {              // On réagit en fonction des événements
        case SDL_QUIT: // si on clique sur la croix on quitte
            fini = true;
            break;
        case SDL_KEYDOWN:
            switch (events.key.keysym.sym)
            {
            case SDLK_ESCAPE: // si on appuie sur échap on quitte
                fini = true;
                break;
            }
            break;
        }

        SDL_Delay(10);
    }

    SDL_DestroyRenderer(rendu); // On quitte proprement
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();

    return 0;
}