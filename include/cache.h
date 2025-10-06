#ifndef CACHE_H
#define CACHE_H

#include "list.h"

#define ERROR 1
#define OK 0
#define UP_TO_DATE 0
#define OUT_OF_DATE -1
#define DOES_NOT_EXIST -2

int Cache_CheckExisting(cityList *city);
int Cache_SaveData(cityList *city, char *httpData);
int Cache_CheckDataAge(const char *filename);

#endif // CACHE_H