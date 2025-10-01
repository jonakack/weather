#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/meteo.h"
#include "../include/http.h"
#include "../include/cities_new.h"  
#include "../include/cache.h"

void display_weather_menu(WeatherData *data)
{
    printf("What would you like to know? \n"
           "[1] Temperature \n"
           "[2] WindSpeed \n"
           "[3] Weather conditions \n"
           "[4] Time \n"
           "[5] Wind direction \n"
           "(n) To cancel\n");

    while (1)
    {
        char line[16];
        if (!fgets(line, sizeof(line), stdin))
            break;
        char choice = line[0];

        // Convert to lowercase manually if uppercase
        if (choice >= 'A' && choice <= 'Z')
        {
            choice = choice + ('a' - 'A');
        }

        if (choice == '1' && data->has_temperature)
        {
            printf("Temperature: %.1f °C\n", data->temperature);
        }
        else if (choice == '2' && data->has_windspeed)
        {
            printf("Windspeed: %.1f m/s\n", data->windspeed);
        }
        else if (choice == '3' && data->has_weathercode)
        {
            char weather_desc[100];
            meteoWeatherCodes(data->weathercode, weather_desc);
            printf("Weather conditions: %s\n", weather_desc);
        }
        else if (choice == '4' && data->has_time)
        {
            printf("Time: %s\n", data->time);
        }
        else if (choice == '5' && data->has_winddirection)
        {
            printf("Wind direction: %d°\n", data->winddirection);
        }
        else if (choice == 'n')
        {
            break;
        }
        else
        {
            printf("Incorrect choice or data missing.\n");
        }
    }
}

// OLD MAIN (using cities[] array)
/*
int main()
{
    char userResponse;
    char url[256];
    int cityIndex = 0;

    build_citystruct();

    do
    {
        printf("-----------------WeatherApp-----------------\n"
               "Select a city by entering the city's number: \n");

        cities_showList();
        cities_choice(&cityIndex);
        makeURL(cityIndex, url);

        char *httpData = http_init(url);
        saveDataHeap(httpData, cityIndex);  // Is not used yet, only stored in memory. Needs more functions.
        saveData(httpData, cityIndex);

        // Parse JSON data and display weather menu to user and free memory afterwards

        WeatherData *weather_data = parse_weather_json(httpData);
        if (weather_data != NULL)
        {
            display_weather_menu(weather_data);
            free_weather_data(weather_data);
        }

        free(httpData);

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (askYesNo(userResponse) == YES);

    free(cities);  // Now we free the allocated memory for cities, since we no longer need it
    printf("Shutting down... Thank you for using WeatherApp!\n");

    return 0;
}
*/

// Updated main (using Cities manager and CityData structs)
int main()
{
    char userResponse;
    char url[256];
    Cities* cities = NULL;

    // Initialize cities this will bootstrap JSON files into cities/ folder if they don't exist
    if(Cities_Init(&cities) != 0)
    {
        fprintf(stderr, "Failed to initialize cities\n");
        return 1; // Exit if cities cannot be initialized
    }

    do
    {
        printf("-----------------WeatherApp-----------------\n");

        // Let user choose a city and get pointer to selected CityData
        CityData* selected_city = NULL;
        if(Cities_Choice(cities, &selected_city) != 0 || selected_city == NULL)
        {
            fprintf(stderr, "Failed to select city\n");
            continue; // Skip to next iteration to allow re-selection
        }

        // Build URL for selected city
        makeURLFromCity(selected_city, url);

        // First, try to load cached weather data
        char *httpData = loadWeatherData(selected_city);

        if(httpData == NULL)
        {
            // No cache or cache is out of date, fetch from API
            printf("Fetching weather data from API...\n");
            httpData = http_init(url);

            if(httpData != NULL)
            {
                // Save the fresh data to cache
                saveWeatherData(selected_city, httpData);
            }
        }

        if(httpData != NULL)
        {
            // Parse JSON data and display weather menu
            WeatherData *weather_data = parse_weather_json(httpData);
            if (weather_data != NULL)
            {
                // Attach weather data to city
                city_attach_weather(selected_city, weather_data);

                display_weather_menu(weather_data);

                // Don't free weather_data here, it's attached to city
                // It will be freed when city is freed
            }

            free(httpData);
        }
        else
        {
            printf("Failed to get weather data\n");
        }

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (askYesNo(userResponse) == YES);

    // Free all cities and the Cities manager
    Cities_Dispose(&cities);
    printf("Shutting down... Thank you for using WeatherApp!\n");

    return 0;
}