#ifndef CITY_H
#define CITY_H

#include <stdbool.h>
#include "json.h"


typedef struct CityData {
    char name[50];
    double latitude;
    double longitude;
    WeatherData *weather_data;  // Parsed weather from JSON
} CityData;

// Core city functions
CityData* city_create(const char *name, double latitude, double longitude);
void city_free(CityData *city);
void city_attach_weather(CityData *city, WeatherData *weather);
void city_print(CityData *city);

// Search functions
CityData* city_find_by_name(const char *name);
CityData* city_find_by_coordinates(double latitude, double longitude);

// Integration functions (connecting cache + json + city)
WeatherData* city_load_weather(CityData *city);  // Load from cache OR fetch
int city_update_weather(CityData *city);         // Force update
bool city_has_weather(CityData *city);           // Check if weather exists

#endif