#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/meteo.h"
#include "../include/http.h"
#include "../include/list.h"

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
        save_data_heap(httpData, cityIndex);  // Is not used yet, only stored in memory. Needs more functions.
        save_data(httpData, cityIndex);

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

    free(cities);
    printf("Shutting down... Thank you for using WeatherApp!\n");
    
    return 0;
}