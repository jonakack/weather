#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/list.h"
#include "../include/cities.h"
#include "../include/http.h"
#include "../include/cache.h"
#include "../include/meteo.h"
#include "../include/libs/utils.h"

// Fetches city from user input, sets first character to uppercase and the rest to lowercase
void Cities_GetChoice(char *_UserChoice)
{
    /* TODO: Fix input buffer bug. Sometimes, after the Input_YesNo function,
    the message "City not found" is shown even if the city name is correct */

    char input[64];
    int len = 0;
    while (scanf("%63s", input) != 1)
    {
        printf("Invalid input\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
        {
            // discard characters
        }
    }

    // Clear remaining input buffer after successful scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        // discard characters
    }

    len = strlen(input);
    int i = 0, j = 0;
    // Uppercase first character (ASCII or UTF-8 Swedish)
    if ((unsigned char)input[i] == 0xc3 && i + 1 < len)
    {
        j += UTF8_SwedishToUpper(&input[i], &_UserChoice[j]);
        i += 2;
    }
    else
    {
        _UserChoice[j++] = toupper((unsigned char)input[i++]);
    }
    // Lowercase the rest (ASCII or UTF-8 Swedish)
    while (i < len)
    {
        if ((unsigned char)input[i] == 0xc3 && i + 1 < len)
        {
            j += UTF8_SwedishToLower(&input[i], &_UserChoice[j]);
            i += 2;
        }
        else
        {
            _UserChoice[j++] = tolower((unsigned char)input[i++]);
        }
    }
    _UserChoice[j] = '\0';
}

// Looks for a local file, if not found or too old: fetches data from url
int Cities_GetData(const char *_Choice)
{
    cityList *chosenCity = List_SearchList((char *)_Choice); // Returns a pointer to the city if it is found
    if (chosenCity == NULL)
    {
        printf("City not found.\n");
        return 1;
    }

    // Check if local file exists
    int check_result = Cache_CheckExisting(chosenCity);
    if (check_result == 0 || check_result == -1)
    {
        printf("Creating new file...\n");
        HTTP_DownloadData(chosenCity);
    }

    Cities_DisplayData(chosenCity->filename, chosenCity->name);

    return 0;
}

// Opens file, parses content and displays it to the user
int Cities_DisplayData(char *_Filename, char *_CityName)
{
    FILE *file = fopen(_Filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *httpData = malloc(file_size + 1);
    if (httpData == NULL)
    {
        printf("Failed to allocate memory\n");
        fclose(file);
        return 1;
    }

    fread(httpData, 1, file_size, file);
    httpData[file_size] = '\0';
    fclose(file);

    JSON_WeatherData *weather_data = JSON_ParseData(httpData);
    if (weather_data != NULL)
    {
        JSON_PrintWeatherData(weather_data, _CityName);
        JSON_FreeWeatherData(weather_data);
    }

    free(httpData);
    return 0;
}
