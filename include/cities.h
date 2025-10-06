#include <stdbool.h>

#ifndef CITIES_H
#define CITIES_H

#define YES 0
#define NO -1

int Cities_GetData(const char *choice);
int Cities_DisplayData(char *filename, char *cityName);
void Cities_GetChoice(char *userChoice);

#endif