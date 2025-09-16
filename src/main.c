#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/weather.h"
#include "../include/http.h"

int main()
{
    char userResponse;
    char url[256];
    build_citystruct();             // Build the city struct array

    do 
    {   
        printf ("-----------------WeatherApp-----------------\n"
                "Select a city by entering the city's number: \n");
        
        cities_showList();          // Show list of available cities
        int city = cities_choice(); // Get user's city choice

        makeURL(city, url);         // Generate the URL for the selected city

        char *json_str = http_init(url);
        if (json_str) {
            //printf("Data received:\n%s\n", json_str); // testa att skriva ut den hämtade JSON-datan !!!DEBUGGING!!!
            parse_weather_json(json_str); // anropa funktionen för att parsa JSON-datan 
            free(json_str); // frigör minnet som allokerats för JSON-strängen
        } else {
            fprintf(stderr, "Failed to get data from URL\n");
        }

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);
        
    }   while (askYesNo(userResponse)==0);

    free(cities);   // Now we free the allocated memory for cities, since we no longer need it
    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}