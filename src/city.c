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

// Attach weather data to city
void city_attach_weather(CityData *city, WeatherData *weather)
{
    if (city == NULL) return;
    
    if (city->weather_data != NULL) {
        free_weather_data(city->weather_data);
    }
    city->weather_data = weather;
}

// Print city with weather
void city_print(CityData *city)
{
    if (city == NULL) {
        printf("Invalid city\n");
        return;
    }
    
    printf("\nCity: %s\n", city->name);
    printf("Coordinates: (%.4f, %.4f)\n", city->latitude, city->longitude);

    if (city->weather_data != NULL) {
        WeatherData *w = city->weather_data;

        if (w->has_temperature)
            printf("Temperature: %.1f°C\n", w->temperature);
        if (w->has_windspeed)
            printf("Wind Speed: %.1f m/s\n", w->windspeed);
        if (w->has_weathercode)
            printf("Weather Code: %d\n", w->weathercode);
        if (w->has_time && w->time)
            printf("Time: %s\n", w->time);
    } else {
        printf("No weather data\n");
    }
    printf("\n");
}

// Check if city has weather
bool city_has_weather(CityData *city)
{
    return city != NULL && city->weather_data != NULL;
}

// Search functions (not implemented yet )
CityData* city_find_by_name(const char *name)
{
    // TODO: Implement when integrated with Cities manager
    (void)name;
    return NULL;
}

CityData* city_find_by_coordinates(double latitude, double longitude)
{
    // TODO: Implement when integrated with Cities manager
    (void)latitude;
    (void)longitude;
    return NULL;
}