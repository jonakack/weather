#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <stdlib.h>
#include "../include/json.h"
#include "../include/meteo.h"
#include "../include/http.h"

int main()
{
    char userResponse;
    char url[256];
    int city = 0;

    build_citystruct();

    do
    {
        printf("-----------------WeatherApp-----------------\n"
               "Select a city by entering the city's number: \n");

        cities_showList();
        cities_choice(&city);
        makeURL(city, url);

        // ---------- Temporary test lines for development ----------

        char *httpData = http_init(url);
        // printf("%s\n\n", httpData);
        saveData(httpData, city);

        // ---------------------------------------------------------

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (askYesNo(userResponse) == YES);

    free(cities); // Now we free the allocated memory for cities, since we no longer need it
    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}