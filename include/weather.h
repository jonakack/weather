#ifndef WEATHER_H
#define WEATHER_H

struct city {
    char name[50]; // Ändra till char* name ksk?
    double latitude;
    double longitude;
};


// Initialize global variables used in multiple functions in cities.c
extern struct city *cities;
extern int city_count;

// Initialize functions used in main.c
void readorcreatefile(const char *url, int city);
void build_citystruct(void);
void cities_showList(void);
int cities_choice(void);
void makeURL(int cityIndex, char *url);
int askYesNo(char a);

#endif