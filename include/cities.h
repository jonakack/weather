#ifndef CITIES_H
#define CITIES_H

struct city {
    char name[50]; // Ändra till char* name ksk?
    double latitude;
    double longitude;
};

// Initialize global variables used in multiple functions in cities.c
extern struct city *cities;
extern int city_count;

// Initialize functions used in main.c
void build_citystruct(void);
void cities_showList(void);
int cities_choice(void);
int askYesNo(char a);

#endif