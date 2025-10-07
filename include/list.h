#ifndef LIST_H
#define LIST_H

#define FOUND 0
#define NOT_FOUND 1

#include <time.h>

typedef struct cityList
{
    char *name;
    double latitude;
    double longitude;
    
    char *filename;

    struct cityList *next;
    struct cityList *prev;
} cityList;



void List_InitCities();
void List_CreateCity(char *name, char *lat, char *lon);
void List_RemoveCityFromList(cityList **head, char* name);
void List_ShowCities();
int List_AddCityToList(cityList *city);
cityList *List_SearchList(char *cityName);
cityList *List_InitCity();

#endif // LIST_H