#ifndef LIST_H
#define LIST_H

#include <time.h>

typedef struct city
{
    char *name;
    float time;

    struct city *next;
    struct city *prev;
} city_t;

// nuvarande list funktioner från list.c
void add_city_to_list(city_t **head, char* name);
void remove_city_from_list(city_t **head, char* name);
void show_list(city_t *head);

#endif