#ifndef CACHE_H
#define CACHE_H

#include "city.h"

#define ERROR 1
#define OK 0
#define UP_TO_DATE 0
#define OUT_OF_DATE -1
#define DOES_NOT_EXIST -2

// First version using cityIndex instead of CityData

/* 
int saveData(char *httpData, int cityIndex);
int saveDataHeap(char *httpData, int index); 
*/

// Updated code using CityData instead of cityIndex
int saveWeatherData(CityData* city, const char* httpData);
char* loadWeatherData(CityData* city);

// Function to check the age of the cached data file
int check_data_age(const char *filename);

#endif // CACHE_H