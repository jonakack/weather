#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/meteo.h"
#include "../include/http.h"
#include <stdlib.h>
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

        //char *json_str = http_init(url);
        read_or_create_file(url, city); // Fetch weather data from file or API if file doesn't exist
        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);
        
    }   while (askYesNo(userResponse)==0);

    free(cities);   // Now we free the allocated memory for cities, since we no longer need it
    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}