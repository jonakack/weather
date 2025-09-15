#ifndef WEATHER_H
#define WEATHER_H

typedef struct
{
    int list;
    char name[16];
    double latitude;
    double longitude;
} city;

extern const int NUMBER_OF_CITIES;

void cities_showList(void);
int cities_choice(void);
char *makeURL(int a);
int askYesNo(char a);

#endif