#ifndef METEO_H
#define METEO_H

#include "cities.h"
#include "cache.h"
#include "city.h"

// OLD API
//void makeURL(int cityIndex, char *url);

// New makeURL using CityData struct instead of cityIndex
void makeURLFromCity(CityData* city, char *url);

// Utility function to get weather conditions from code
void meteoWeatherCodes(int code, char *desc);

#endif