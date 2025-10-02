#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/cities.h"
#include "../include/list.h"

int main()
{
    char userResponse;
    char userChoice[32];

    load_city_list();

    do
    {
        printf("-----------------WeatherApp-----------------\n"
               "Select a city by entering the city's name: \n");

        show_cities_list();

        if (scanf("%31s", userChoice) != 1)
        {
            printf("Invalid input\n");
            continue;
        }
        get_city_data(userChoice);

        printf("\nDo you want to select another city?\nEnter Y/N:\n");
        scanf(" %c", &userResponse);

    } while (ask_yes_or_no(userResponse) == YES);

    printf("Shutting down... Thank you for using WeatherApp!\n");

    return 0;
}