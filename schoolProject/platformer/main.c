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

    bool tropPetit = false; //boolean qui retourne si le niveau est trop petit

    char* pseudo = malloc(sizeof(char)*20); //Pseudo (max 20 charactères)

    printf("Entrez votre pseudo (max 20 charactères) : \n");
    scanf("%s", pseudo);

    int tailleX = 0; //taille du tableau du niveau
    int tailleY = 0;

    int currentLevelName = 0; //index du tableau des niveaux
    int nbLevels = 3; //Nombre de levels

    char *levelNames[nbLevels]; //Tableau contenant les niveaux disponibles
    levelNames[0] = "niveau1.txt";
    levelNames[1] = "niveau2.txt";
    levelNames[2] = "niveau3.txt";

    char *path = "niveaux/";                                             // chemin vers le dossier
    char *completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
    char **currentLevel = lire_fichier(completePath);                    // lecture du niveau puis on le met dans un tableau

    taille_fichier(completePath, &tailleY, &tailleX); // récupération de la taille du fichier

    if(tailleY <= 3 || tailleX <= 3){ //on regarde si le niveau est un minimum grand
        printf("Erreur taille de niveau trop petit \n");
        tropPetit = true;
    }

    int nbCle = 0;
    int comptCle = 0;

    SDL_Rect* pickups = malloc(sizeof(SDL_Rect) * 3); //tableau contenant les clés dans un niveau, le max de clé dans un niveau étant 3 (et le min 0)
    currentLevel = modifier_caractere(currentLevel, pickups, &nbCle, tailleX, tailleY, '4', ' '); //appel de la fonction qui va mettre les clés dans le tableau des clés

    const Uint8 *keyStates;

    bool mort = false;
    bool fini = false;
    bool collision = false;
    bool saut = false;
    bool collisionSol = false;
    bool finNiveau = false;

    int tempSaut = 0;
    int sens = 0;
    int sensIdle = 1;
    int tempsSortie = 0;

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
    p_rect.y = (tailleY * 32) - 96;
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

    while (!fini && !tropPetit)
    { // Boucle principal

        //**----------------------------------------Mouvements--------------------------------------------------**

        keyStates = SDL_GetKeyboardState(NULL); //On récupére l'état actuel du clavier (si une touche est pressée ou non)

        mouvements(keyStates, &p_rect, &sens, &tempSaut, &saut); //On bouge le personnage en fonction des touches pressées

        mouvementCollisionSol(p_rect, &sol_Collision); // On bouge le rectangle qui détecte la collision avec le sol, pour le mettre sous le joueur

        //**----------------------------------------Collisions--------------------------------------------------**

        for (int i = 0; i < tailleX; i++)
        { // On regarde si le joueur est entré en collision avec le décor et on récupére la collision dans le rectangle "overlap"
            for (int j = 0; j < tailleY; j++)
            {
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '1') //contact avec les murs
                {
                    collision = true;
                    SDL_IntersectRect(&p_rect, &r_rect[i][0], &overlap);
                }
                if (SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '1') //contact des pieds avec les murs
                {
                    collisionSol = true;
                    niveauCollision = (j * 32) - 32;
                }
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '3' && comptCle == nbCle) //contact sortie
                {
                    tempsSortie++;
                    if(tempsSortie == TEMPS_FIN_NIVEAU){
                        finNiveau = true;
                    }
                }
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '5') //contact avec des piques
                {
                    mort = true;
                }
                if (SDL_HasIntersection(&p_rect, &r_rect[i][j]) && currentLevel[i][j] == '2') //contact avec les briques de laves
                {
                    mort = true;
                }
            }
        }

        //Collisions avec les clés
        for(int i = 0; i < nbCle; i++){
            if(SDL_HasIntersection(&p_rect, &pickups[i])){
                pickups[i].h = 0;
                pickups[i].w = 0;
                comptCle++;
            }
        }


        //On regarde les collisions avec le sol, via le rectangle sous les pieds du joueur
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

        //**----------------------------------------Rendu--------------------------------------------------**

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
                    if(comptCle >= nbCle){
                        SDL_RenderCopy(rendu, ressources->sols[3], NULL, &r_rect[i][j]);
                    }else{
                        SDL_RenderCopy(rendu, ressources->sols[4], NULL, &r_rect[i][j]);
                    }
                    break;
                case '5':
                    SDL_RenderCopy(rendu, ressources->sols[5], NULL, &r_rect[i][j]);
                    break;
                default:
                    SDL_RenderCopy(rendu, ressources->sols[0], NULL, &r_rect[i][j]);
                }
                // printf("i/j = %d/%d\n", i,j);
            }
        }

        //On dessine les clés
        for(int i = 0; i < nbCle; i++){
            SDL_RenderCopy(rendu, ressources->cle, NULL, &pickups[i]);
        }

        switch (sens)
        { // on applique la texture du joueur à la position du rectangle du joueur dans le sens du joueur
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

        //**--------------------------------------Etats du jeu----------------------------------------------**

        //On regarde si le joueur est tombé dans le vide

        if(p_rect.y > tailleY * 32){
            mort = true;
        }

        //si le joueur est mort on recharge le niveau
        if(mort){

            mort = false;

            completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
            printf("%s \n", completePath);
            currentLevel = lire_fichier(completePath); // lecture du niveau puis on le met dans un tableau

            nbCle = 0; //On remet à 0 le nb de clé dans le niveau et le nombre de clé récupérés
            comptCle = 0;

            currentLevel = modifier_caractere(currentLevel, pickups, &nbCle, tailleX, tailleY, '4', ' '); //On recharge les clés mise dans le niveau

            apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 32, 200, 80, "YOU DIED");
            SDL_RenderPresent(rendu);

            SDL_Delay(2000);

            p_rect.x = 32 * 2; //Remettre le joueur au début du niveau
            p_rect.y = (tailleY * 32) - 96;

        }

        if (finNiveau) //Si on fini le niveau on charge le suivant (ou on fini le jeu)
        {

            currentLevelName++;

            if(currentLevelName == nbLevels){ //On regarde si le nombre de niveaux fini correspond au nombre de niveaux dans le jeu pour savoir si le joueur à fini le jeu
                apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 32, 200, 80, "You won");
                SDL_RenderPresent(rendu);

                SDL_Delay(2000);

                apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 28, 200, 80, "Restart"); 
                apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 38, 200, 80, "Quit");
                apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 20, 200, 40, "Cliquez sur votre choix");
                SDL_RenderPresent(rendu);

                bool clique = false;
                SDL_Event cliqueEvents;

                while(!clique){ //On ouvre le menu pour savoir si le joueur veut quitter ou recommencer
                    SDL_WaitEvent(&cliqueEvents);

                    switch(cliqueEvents.type){
                        case SDL_MOUSEBUTTONDOWN:
                            case SDL_BUTTON_LEFT:
                            if(cliqueEvents.button.x < ((tailleX / 2) * 32 - 100) + 200 && cliqueEvents.button.x > (tailleX / 2) * 32 - 100){
                                if(cliqueEvents.button.y < ((tailleY / 2) * 38) + 80 && cliqueEvents.button.y > (tailleY / 2) * 38){
                                    fini = true;
                                    clique = true;
                                }
                                if(cliqueEvents.button.y < ((tailleY / 2) * 28) + 80 && cliqueEvents.button.y > (tailleY / 2) * 28){
                                    clique = true;
                                    currentLevelName = 0;

                                    tempsSortie = 0;

                                    completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
                                    printf("%s \n", completePath);
                                    currentLevel = lire_fichier(completePath); // lecture du niveau puis on le met dans un tableau

                                    nbCle = 0; //On remet à 0 le nb de clé dans le niveau et le nombre de clé récupérés
                                    comptCle = 0;

                                    currentLevel = modifier_caractere(currentLevel, pickups, &nbCle, tailleX, tailleY, '4', ' '); //On recharge les clés mise dans le niveau

                                    p_rect.x = 32 * 2; //Remettre le joueur au début du niveau
                                    p_rect.y = (tailleY * 32) - 96;

                                    finNiveau = false;
                                }
                            }
                        break;
                    }
                SDL_Delay(10);
                }
            }else{

                tempsSortie = 0;

                completePath = easyStrcat(path, levelNames[currentLevelName]); // chemin vers le fichier du niveau
                printf("%s \n", completePath);
                currentLevel = lire_fichier(completePath); // lecture du niveau puis on le met dans un tableau

                nbCle = 0; //On remet à 0 le nb de clé dans le niveau et le nombre de clé récupérés
                comptCle = 0;

                currentLevel = modifier_caractere(currentLevel, pickups, &nbCle, tailleX, tailleY, '4', ' '); //On recharge les clés mise dans le niveau

                p_rect.x = 32 * 2; //Remettre le joueur au début du niveau
                p_rect.y = (tailleY * 32) - 96;

                finNiveau = false;

                apply_text(rendu, (tailleX / 2) * 32 - 100, (tailleY / 2) * 32, 200, 80, "NEXT LEVEL");
                SDL_RenderPresent(rendu);

                SDL_Delay(2000);
            }
        }

        SDL_RenderPresent(rendu);

        //**------------------------------------------------------Evenements------------------------------------------------**

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

    ecrirePseudo(pseudo);

    free(pseudo); //On quitte proprement
    free(pickups);
    free(ressources);
    free(r_rect);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    TTF_Quit();
    SDL_Quit();

    return 0;
}