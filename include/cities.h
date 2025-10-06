#include <stdbool.h>

#ifndef CITIES_H
#define CITIES_H

#define YES 0
#define NO -1

int ask_yes_or_no(char choice);
int get_city_data(const char *choice);
int show_weather_data(char *filename, char *cityName);
void get_city_choice(char *userChoice);

#endif