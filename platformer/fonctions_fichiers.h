#ifndef FONCTIONS_FICHIERS
#define FONCTIONS_FICHIERS

void taille_fichier(const char* nomFichier, int* nbLig, int* nbCol);
char** lire_fichier(const char* nomFichier);
void ecrire_fichier(const char* nomFichier, char** tab, int n, int m);
char** modifier_caractere(char** tab, int n, int m, char ancien, char nouveau);

#endif