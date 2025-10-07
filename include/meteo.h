#ifndef METEO_H
#define METEO_H

#include "cities.h"
#include "cache.h"
#include "list.h"
#include "json.h"

// Initialize functions defined in meteo.c
char *Meteo_MakeURL(double _Latitude, double _Longitude);
void Meteo_ShowWeatherCode(int _WeatherCode, char *_Description);

#endif