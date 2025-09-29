#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/meteo.h"
#include "../include/http.h"
#include "../include/list.h"

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
        saveDataHeap(httpData, cityIndex); //  Is not used yet, only stored in memory. Needs more functions. 
        saveData(httpData, cityIndex);
        parse_weather_json(httpData);

        free(httpData);

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (askYesNo(userResponse) == YES);

    free(cities); // Now we free the allocated memory for cities, since we no longer need it
    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}