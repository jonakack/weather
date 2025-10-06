#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "../include/libs/utils.h"
#include "../include/cities.h"
#include "../include/list.h"

int main()
{
    char userResponse;
    char cityName[64];

    // Set locale for proper UTF-8 handling
    setlocale(LC_ALL, "");

    List_InitCities();

    do
    {
        printf("-----------------WeatherApp-----------------\n"
               "Select a city by entering the city's name: \n");
        
        List_ShowCities();

        Cities_GetChoice(cityName);
        Cities_GetData(cityName);

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (Input_YesNo(userResponse) == YES);

    printf("Shutting down... Thank you for using WeatherApp!\n");

    return 0;
}