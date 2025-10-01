#ifndef CITIES_NEW_H
#define CITIES_NEW_H

#include <stdbool.h>
#include "utils/LinkedList.h"
#include "city.h"

// Return codes for Cities operations
typedef enum CitiesResult
{
    CITIES_SUCCESS = 0,
    CITIES_ERROR = -1,
    CITIES_NOT_FOUND = -2
} CitiesResult;

#define YES 0
#define NO -1

// Cities manager structure with linked list
typedef struct Cities
{
    LinkedList list;
} Cities;

// Initialize cities manager and load default cities
int Cities_Init(Cities** _CitiesPtr);

// Add cities from string list (format: "Name:Lat:Lon\n...")
void Cities_AddFromStringList(Cities* _Cities, const char* _StringList);

// Create and add a new city
int Cities_Create(Cities* _Cities, const char* _Name, const char* _Latitude, const char* _Longitude, CityData** _CityPtr);

// Get city by name
int Cities_GetByName(Cities* _Cities, const char* _Name, CityData** _CityPtr);

// Get city by index (0-based)
int Cities_GetByIndex(Cities* _Cities, int _Index, CityData** _CityPtr);

// Remove and destroy a city
void Cities_Destroy(Cities* _Cities, CityData** _CityPtr);

// Print all cities
void Cities_Print(Cities* _Cities);

// User choice (interactive)
int Cities_Choice(Cities* _Cities, CityData** _CityPtr);

// Free all cities and manager
void Cities_Dispose(Cities** _CitiesPtr);

// Helper function
int askYesNo(char a);

#endif // CITIES_NEW_H