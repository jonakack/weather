#ifndef LIST_H
#define LIST_H

#include <time.h>

typedef struct cityList
{
    char *name;
    float time;

    struct cityList *next;
    struct cityList *prev;
} cityList; 

// cityList *head = NULL;
// cityList *tail = NULL;

// Initialize functions for linked list
void add_city_to_list(cityList **head, char* name);
void remove_city_from_list(cityList **head, char* name);
void show_list(cityList *head);

#endif