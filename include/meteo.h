#ifndef METEO_H
#define METEO_H

#include "cities.h"
#include "cache.h"
#include "list.h"
#include "json.h"

// Initialize functions defined in meteo.c
char *makeURL(double latitude, double longitude);
void meteoWeatherCodes(int code, char *desc);
char *http_init(const char *url);
WeatherData *parse_weather_json(const char *json);
int download_city_data(cityList *city);

#endif