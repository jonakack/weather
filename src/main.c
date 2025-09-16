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
    char url[MAX_URL_LENGTH]; /* Using defined constant instead of magic number */

    do 
    {   
        printf ("-----------------WeatherApp-----------------\n"
                "Select a city by entering the city's number: \n");

        cities_showList();

        int city = cities_choice();

        /* validates makeURL return and use safe string copying to prevent buffer overflow */
        const char *temp_url = makeURL(city);
        if (temp_url == NULL) {
            fprintf(stderr, "Error: Could not generate URL for city\n");
            continue;
        }
        strncpy(url, temp_url, MAX_URL_LENGTH - 1);
        url[MAX_URL_LENGTH - 1] = '\0'; /* Ensure null-termination for safety */

        char *json_str = http_init(url);
        
        if (json_str) {
                parse_weather_json(json_str); /* anropa funktionen för att parsa JSON-datan */
                /*printf("Data received:\n%s\n", json_str); // testa att skriva ut den hämtade JSON-datan !!!DEBUGGING!!! */
                free(json_str); /* frigör minnet som allokerats för JSON-strängen */
        } else 
            {
            fprintf(stderr, "Failed to get data from URL\n");
            }

        printf("\n\nDo you want to select another city?\nEnter Y/N:\n");
        /* Enhanced error handling for user input to prevent input stream corruption */
        if (scanf(" %c", &userResponse) != 1) {
            fprintf(stderr, "Error reading user input\n");
            /* Clear input buffer and default to exit */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            userResponse = 'N';
        }
        
    }   while (askYesNo(userResponse)==YES_RESPONSE); /* Use named constant instead of magic number */

    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}