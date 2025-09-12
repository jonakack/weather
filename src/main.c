#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "../include/weather.h"
#include "../include/http.h"

int main()
{
    char c;
    char url[256];

    do 
    {   
        printf ("-----------------WeatherApp-----------------\n"
                "Select a city by entering the city's number: \n");

        cities_showList();

        int city = cities_choice();

        strcpy(url, makeURL(city));

        http_init(url);
        
        printf("\n\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &c);
        
    }   while (selection(c)==0);

    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}