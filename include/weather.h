#ifndef WEATHER_H
#define WEATHER_H

/* Constants for buffer sizes and limits */
#define MAX_CITY_NAME_LENGTH 16
#define MAX_URL_LENGTH 256
#define MAX_INPUT_LINE_LENGTH 16

/* Response constants for user input validation */
#define YES_RESPONSE 0
#define NO_RESPONSE 1

typedef struct
{
    int list;
    char name[MAX_CITY_NAME_LENGTH];
    double latitude;
    double longitude;
} city;

int get_number_of_cities(void);
void cities_showList(void);
int cities_choice(void);
char *makeURL(int a);
int askYesNo(char a);

#endif