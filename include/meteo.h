#ifndef METEO_H
#define METEO_H

#include "cities.h"
#include "cache.h"

// meteo relaterade funktioner
void makeURL(int cityIndex, char *url);
void meteoWeatherCodes(int code, char *desc);

#endif