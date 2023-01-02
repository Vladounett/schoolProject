#include "fonctions_physique.h"

//bouge le rectangle de collision avec le sol sous les pieds du joueur

void mouvementCollisionSol(SDL_Rect p_rect, SDL_Rect *s_rect)
{
    s_rect->x = p_rect.x + 4;
    s_rect->y = p_rect.y + 30;
}

void mouvements(const Uint8 *keyStates, SDL_Rect *p_rect, int *sens, int *tempSaut, bool *saut)
{

    // On déplace le personnage en fonction des touches pressées

    if (keyStates[SDL_SCANCODE_A])
    {
        p_rect->x -= PLAYER_SPEED;
        *sens = -1;
    }
    else if (keyStates[SDL_SCANCODE_D])
    {
        p_rect->x += PLAYER_SPEED;
        *sens = 1;
    }
    else
    {
        *sens = 0;
    }
    if (keyStates[SDL_SCANCODE_SPACE] && !*saut)
    {
        *tempSaut = 0;
        *saut = true;
    }

    if (*saut && *tempSaut < PLAYER_JUMP_TIME)
    {
        p_rect->y -= PLAYER_JUMP;
        *tempSaut += 1;
    }
    else
    {
        p_rect->y += GRAVITY;
    }
}

/*void handle_collisions(int tailleX, int tailleY, char** currentLevel, SDL_Rect* p_rect, SDL_Rect** r_rect, SDL_Rect sol_Collision, bool* collision, bool* collisionSol, bool* saut, bool* finNiveau)
{

    //################################
    //NE FONCTIONNE PAS POUR L'INSTANT
    //################################

    int niveauCollision = 0;
    SDL_Rect overlap;

    // Collisions :

    for (int i = 0; i < tailleX; i++)
    { // On regarde si le joueur est entré en collision avec le décor et on récupére la collision dans le rectangle "overlap"
        for (int j = 0; j < tailleY; j++)
        {
            if (SDL_HasIntersection(p_rect, &r_rect[i][j]) && currentLevel[i][j] == '1')
            {
                *collision = true;
                SDL_IntersectRect(p_rect, &r_rect[i][0], &overlap);
            }
            if (SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '1')
            {
                *collisionSol = true;
                niveauCollision = (j * 32) - 32;
            }
            if (SDL_HasIntersection(p_rect, &r_rect[i][j]) && currentLevel[i][j] == '3')
            {
                *finNiveau = true;
            }
            if (SDL_HasIntersection(&sol_Collision, &r_rect[i][j]) && currentLevel[i][j] == '3')
            {
                *finNiveau = true;
            }
        }
    }

    if (collisionSol)
    {
        p_rect->y = niveauCollision;
        *collisionSol = false;
        *saut = false;
        mouvementCollisionSol(*p_rect, &sol_Collision);
    
    }
    if (collision && overlap.x > p_rect->x)
    { // Si le joueur est entré en collision on l'enlève de la dite collision
        p_rect->x -= PLAYER_SPEED;
        *collision = false;
        overlap.w = 0;
        overlap.h = 0;
        mouvementCollisionSol(*p_rect, &sol_Collision);
    }
    else if (collision && overlap.x <= p_rect->x)
    {
        p_rect->x += PLAYER_SPEED;
        *collision = false;
        overlap.w = 0;
        overlap.h = 0;
        mouvementCollisionSol(*p_rect, &sol_Collision);
    }    
}*/