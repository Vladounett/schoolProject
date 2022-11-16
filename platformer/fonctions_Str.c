#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include "fonctions_Str.h"


char* easyStrcat(char* str1, char* str2){

    char* strReturn = malloc(sizeof(char) * strlen(str1) * strlen(str2));
    strcat(strReturn, str1);
    strcat(strReturn, str2);

    return strReturn;
}