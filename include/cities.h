#include <stdbool.h>

#ifndef CITIES_H
#define CITIES_H

#define YES 0
#define NO -1

struct city
{
    char name[50]; // Ändra till char* name ksk?
    double latitude;
    double longitude;
    char *content;
    bool savedOrNot;
};

extern struct city *cities;
extern int city_count;

// Initialize functions used in main.c
void build_citystruct(void);
void cities_showList(void);
void cities_choice(int *cityIndexChoice);
int askYesNo(char a);

#endif