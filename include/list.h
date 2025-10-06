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



void build_cities_list(char *stringList);
void create_city(char *name, char *lat, char *lon);
void remove_city_from_list(cityList **head, char* name);
void show_cities_list();
int load_city_list();
int add_city_to_list(cityList *city);
cityList *search_list(char *cityName);
cityList *initialize_city();

#endif // LIST_H