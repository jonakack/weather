#ifndef WEATHER_H
#define WEATHER_H

/* Constants for buffer sizes and limits */
#define MAX_CITY_NAME_LENGTH 16
#define MAX_URL_LENGTH 256
#define MAX_INPUT_LINE_LENGTH 16

typedef struct
{
    int list;
    char name[MAX_CITY_NAME_LENGTH];
    double latitude;
    double longitude;
} city;

extern const int NUMBER_OF_CITIES;

void cities_showList(void);
int cities_choice(void);
char *makeURL(int a);
int askYesNo(char a);

#endif