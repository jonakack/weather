#ifndef METEO_H
#define METEO_H

#include "cities.h"
#include "cache.h"

// Initialize functions defined in meteo.c
void makeURL(int cityIndex, char *url);
void meteoWeatherCodes(int code, char *desc);

#endif