#include <stdio.h>
#include "../include/weather.h"
#include <curl/curl.h>

int main()
{
    char c;

    do 
    {   
        printf ("-----------------WeatherApp-----------------\n"
                "Select a city by entering the city's number: \n");

        cities_showList();

        int city = cities_choice();

        printURL(city);
        
        printf("Do you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &c);
        
    }   while (selection(c)==0);

    printf("Shutting down... Thank you for using WeatherApp!\n");
    return 0;
}