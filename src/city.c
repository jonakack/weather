#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/city.h"
#include "../include/json.h"

// Create a new city 
CityData* city_create(const char* name, double latitute, double longitude)
{
    CityData* city = (CityData*)malloc(sizeof(CityData));
    if (!city) {
        fprintf(stderr, "Memory allocation failed for CityData\n");
        return NULL;
    }
    strncpy(city->name, name, sizeof(city->name) - 1);
    city->name[sizeof(city->name) - 1] = '\0'; // Ensure null-termination
    city->latitude = latitute;
    city->longitude = longitude;
    city->weather_data = NULL; // Initialize weather_data to NULL
    return city;
}

// Free a city 
void city_free(CityData *city)
{
    if (city == NULL) return;
    if (city->weather_data != NULL) {
        free_weather_data(city->weather_data);
}
    free(city);
}

// NOT DONE YET 

// void city_attach_weather(CityData *city, WeatherData *weather);
// void city_print(CityData *city);

// Search functions

// CityData* city_find_by_name(const char *name);
// CityData* city_find_by_coordinates(double latitude, double longitude);

// Integration functions (connecting cache + json + city)
// WeatherData* city_load_weather(CityData *city);  // Load from cache OR fetch
// int city_update_weather(CityData *city);         // Force update
// bool city_has_weather(CityData *city);           // Check if weather exists