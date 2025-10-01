#define _POSIX_C_SOURCE 200809L // For strdup on Linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

#include "../include/cities_new.h"
#include "../include/city.h"
#include "../include/cJSON.h"

// Macro to make mkdir work on Windows and Linux
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#define MKDIR(path) mkdir(path, 0777)
#endif

// Default cities list
const char* Cities_list = 	"Stockholm:59.3293:18.0686\n"
							"Göteborg:57.7089:11.9746\n"
							"Malmö:55.6050:13.0038\n"
							"Uppsala:59.8586:17.6389\n"
							"Västerås:59.6099:16.5448\n"
							"Örebro:59.2741:15.2066\n"
							"Linköping:58.4109:15.6216\n"
							"Helsingborg:56.0465:12.6945\n"
							"Jönköping:57.7815:14.1562\n"
							"Norrköping:58.5877:16.1924\n"
							"Lund:55.7047:13.1910\n"
							"Gävle:60.6749:17.1413\n"
							"Sundsvall:62.3908:17.3069\n"
							"Umeå:63.8258:20.2630\n"
							"Luleå:65.5848:22.1567\n"
							"Kiruna:67.8558:20.2253\n"
							"Skellefteå:64.7506:20.9528\n";

int Cities_Init(Cities** _CitiesPtr)
{
	if(_CitiesPtr == NULL)
		return CITIES_ERROR;

	Cities* _Cities = (Cities*)malloc(sizeof(Cities));
	if(_Cities == NULL)
		return CITIES_ERROR;

	LinkedList_Initialize(&_Cities->list);

	Cities_AddFromStringList(_Cities, Cities_list);

	*(_CitiesPtr) = _Cities;
	return CITIES_SUCCESS;
}

// Helper function to save city metadata to JSON file, only if it doesn't exist
static int Cities_SaveCityMetadataToJSON(CityData* city)
{
	if(city == NULL)
		return -1; // Invalid argument for the function to save city metadata

	// Create cities directory if it doesn't exist
	if (MKDIR("cities") != 0)
	{
		if (errno != EEXIST)
		{
			perror("Failed to create citiesS directory: Permission denied");
			return -2;
		}
	}

	// Create filename: cities/CityName_lat_lon.json
	char filename[128];
	snprintf(filename, sizeof(filename), "cities/%s_%.2f_%.2f.json",
		city->name, city->latitude, city->longitude);

	// Check if file already exists
	struct stat file_stat;
	if(stat(filename, &file_stat) == 0)
	{
		// File already exists, skip creation
		return 0;
	}

	// Create JSON object with city metadata
	cJSON *json_city = cJSON_CreateObject();
	if(json_city == NULL)
		return -3;

	cJSON_AddStringToObject(json_city, "name", city->name);
	cJSON_AddNumberToObject(json_city, "latitude", city->latitude);
	cJSON_AddNumberToObject(json_city, "longitude", city->longitude);

	// Convert to string
	char *json_string = cJSON_Print(json_city);
	if(json_string == NULL)
	{
		cJSON_Delete(json_city);
		return -4;
	}

	// Write to file
	FILE *fp = fopen(filename, "w");
	if(fp == NULL)
	{
		fprintf(stderr, "Failed to create city file: %s\n", filename);
		free(json_string);
		cJSON_Delete(json_city);
		return -5;
	}

	fprintf(fp, "%s", json_string);
	fclose(fp);

	// Cleanup
	free(json_string);
	cJSON_Delete(json_city);

	return 0;
}

void Cities_AddFromStringList(Cities* _Cities, const char* _StringList)
{
	if(_Cities == NULL || _StringList == NULL)
		return;

	char* list_copy = strdup(_StringList);
	if(list_copy == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for list copy\n");
		return;
	}

	char* ptr = list_copy;

	char* name = NULL;
	char* lat_str = NULL;
	char* lon_str = NULL;
	do
	{
		if(name == NULL)
		{
			name = ptr;
		}
		else if(lat_str == NULL)
		{
			if(*(ptr) == ':')
			{
				lat_str = ptr + 1;
				*(ptr) = '\0';
			}
		}
		else if(lon_str == NULL)
		{
			if(*(ptr) == ':')
			{
				lon_str = ptr + 1;
				*(ptr) = '\0';
			}
		}
		else
		{
			if(*(ptr) == '\n')
			{
				*(ptr) = '\0';

				CityData* new_city = NULL;
				Cities_Create(_Cities, name, lat_str, lon_str, &new_city);

				// Bootstrap: Save city metadata to JSON file 
				if(new_city != NULL)
				{
					Cities_SaveCityMetadataToJSON(new_city);
				}

				name = NULL;
				lat_str = NULL;
				lon_str = NULL;
			}
		}

		ptr++;

	} while (*(ptr) != '\0');

	free(list_copy);
}

int Cities_Create(Cities* _Cities, const char* _Name, const char* _Latitude, const char* _Longitude, CityData** _CityPtr)
{
	if(_Cities == NULL || _Name == NULL)
		return -1;

	double lat = atof(_Latitude);
	double lon = atof(_Longitude);

	CityData* new_city = city_create(_Name, lat, lon);
	if(new_city == NULL)
	{
		fprintf(stderr, "Failed to create city: %s\n", _Name);
		return -2;
	}

	LinkedList_Push(&_Cities->list, new_city);

	if(_CityPtr != NULL)
		*(_CityPtr) = new_city;

	return 0;
}

int Cities_GetByName(Cities* _Cities, const char* _Name, CityData** _CityPtr)
{
	if(_Cities == NULL || _Name == NULL || _CityPtr == NULL)
		return -1;

	CityData* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		if(strcmp(city->name, _Name) == 0)
		{
			*(_CityPtr) = city;
			return 0;
		}
	}

	return -2;
}

int Cities_GetByIndex(Cities* _Cities, int _Index, CityData** _CityPtr)
{
	if(_Cities == NULL || _CityPtr == NULL)
		return -1;

	if(_Index < 0 || _Index >= _Cities->list.length)
		return -2;

	CityData* city = (CityData*)LinkedList_Get(&_Cities->list, _Index);
	if(city == NULL)
		return -3;

	*(_CityPtr) = city;

	return 0;
}

void Cities_Destroy(Cities* _Cities, CityData** _CityPtr)
{
	if(_Cities == NULL || _CityPtr == NULL || *(_CityPtr) == NULL)
		return;

	CityData* city = *(_CityPtr);
	LinkedList_Remove(&_Cities->list, city);
	city_free(city);

	*(_CityPtr) = NULL;
}

void Cities_Print(Cities* _Cities)
{
	if(_Cities == NULL)
		return;

	printf("\nAvailable Cities:\n");
	int index = 1;
	CityData* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		printf("[%d] %s\n", index++, city->name);
	}
	printf("\n");
}

int Cities_Choice(Cities* _Cities, CityData** _CityPtr)
{
	if(_Cities == NULL || _CityPtr == NULL)
		return -1;

	Cities_Print(_Cities);

	printf("Select a city by number: ");
	int choice = 0;

	while(1)
	{
		if(scanf("%d", &choice) == 1 && choice >= 1 && choice <= _Cities->list.length)
		{
			while(getchar() != '\n');
			break;
		}
		else
		{
			printf("Invalid choice. Try again (1-%d): ", _Cities->list.length);
			while(getchar() != '\n');
		}
	}

	int result = Cities_GetByIndex(_Cities, choice - 1, _CityPtr);
	if(result == 0 && *_CityPtr != NULL)
	{
		printf("\nYou chose: %s\n", (*_CityPtr)->name);
	}

	return result;
}

void Cities_Dispose(Cities** _CitiesPtr)
{
	if(_CitiesPtr == NULL || *(_CitiesPtr) == NULL)
		return;

	Cities* _Cities = *(_CitiesPtr);

	CityData* city = NULL;
	LinkedList_ForEach(&_Cities->list, &city)
	{
		city_free(city);
	}

	LinkedList_Dispose(&_Cities->list);

	free(_Cities);

	*(_CitiesPtr) = NULL;
}

// Note: askYesNo() is defined in cities.c (legacy)
