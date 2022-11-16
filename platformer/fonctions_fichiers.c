#include "fonctions_fichiers.h"
#include "stdio.h"
#include "stdlib.h"

//taille_fichier qui modifie nbLig et nbCol en fonction du nombre de lignes et de colonnes du fichier

void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol){

    FILE* fichier = fopen(nomFichier, "r");

    char c;

    int lignes = 1;
    int colones = 0;
    int maxCol = 0;

    while(!feof(fichier)){

        c = fgetc(fichier);

        if(c == '\n'){

            lignes++;
            colones = 0;

        } else {
            colones++;
        }

        if(maxCol < colones){
            maxCol = colones;
        }
    }

    *nbLig = lignes;
    *nbCol = maxCol-1;

    fclose(fichier); 
}

//lire_fichier, lit un .txt et le met dans un tableau char par char

char** lire_fichier(const char* nomFichier){
    FILE* fichier = fopen(nomFichier, "r");

    int nbLig; int nbCol;
    int i = 0; int j = 0;
    char c;

    taille_fichier(nomFichier, &nbLig, &nbCol);

    char** tab = malloc(sizeof(char*) * nbCol);
    for(int i = 0; i < nbCol; i++){
        tab[i] = malloc(sizeof(char) * nbLig);
    }

    for(int i = 0; i < nbCol; i++){
        tab[i][0] = ' ';
        for(int j = 0; j < nbLig; j++){
            tab[i][j] = ' ';
        }
    }

    while(!feof(fichier)){

        c = fgetc(fichier);

        if(c != EOF && c != '\n' && c != '\r'){
            tab[i][j] = c;
            //printf("tab[%d][%d] = %c \n",i,j,tab[i][j]);
        }

        if(c == '\n'){
            j++;
            i = 0;
        } else {
            i++;
        }
    }

    fclose(fichier);

    return tab;
}

//ecrire_fichier, écrit un tableau char** dans un .txt

void ecrire_fichier(const char* nomFichier, char** tab, int n, int m){

    FILE* fichier = fopen(nomFichier, "w");

    int i = 0; int j = 0;

    while(j < m){

        //printf("tab[%d][%d] = %c \n",i,j,tab[i][j]);
        fputc(tab[i][j], fichier);

        if(i < n - 1){
            i++;
        } else {
            j++;
            i = 0;
            fputc('\n', fichier);
        }
    }

    fclose(fichier);
}

//remplace certains char d'un tableau de char** par un autre char

char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau){

    for(int i = 0; i < n; i++){
        if(tab[i][0] == ancien){
            tab[i][0] = nouveau;
        }
        for(int j = 0; j < m; j++){
            if(tab[i][j] == ancien){
                tab[i][j] = nouveau;
            }
        }
    }

    return tab;
}