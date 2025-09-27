#ifndef CITIES_H
#define CITIES_H

#define YES 0

struct city
{
    char name[50]; // Ändra till char* name ksk?
    double latitude;
    double longitude;
};

extern struct city *cities;
extern int city_count;

// Initialize functions used in main.c
void build_citystruct(void);
void cities_showList(void);
void cities_choice(int *cityIndexChoice);
int askYesNo(char a);
int saveData(char *httpData, int cityIndex);

#endif