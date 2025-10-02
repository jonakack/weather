#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"
#include "../include/cities.h"
#include "../include/http.h"
#include "../include/cache.h"
#include "../include/meteo.h"

int get_city_data(const char *choice)
{
    cityList *chosenCity = search_list((char *)choice); // returns a pointer to the city if it is found
    if (chosenCity == NULL)
    {
        printf("City not found.\n");
        return 1;
    }

    // Check if local file exists
    int check_result = check_existing(chosenCity);
    if (check_result == 0 || check_result == ERROR)
    {
        download_city_data(chosenCity);
    }

    show_weather_data(chosenCity->filename, chosenCity->name);

    return 0;
}

int show_weather_data(char *filename, char *cityName)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory for file content
    char *httpData = malloc(file_size + 1);
    if (httpData == NULL)
    {
        printf("Failed to allocate memory\n");
        fclose(file);
        return 1;
    }

    // Read file content
    fread(httpData, 1, file_size, file);
    httpData[file_size] = '\0'; // Null terminate
    fclose(file);

    WeatherData *weather_data = parse_weather_json(httpData);
    if (weather_data != NULL)
    {
        display_weather_menu(weather_data, cityName);
        free_weather_data(weather_data);
    }

    free(httpData);
    return 0;
}

int ask_yes_or_no(char choice)
{
    while (1)
    {
        if (choice == 'Y' || choice == 'y')
        {
            return 0;
        }
        else if (choice == 'N' || choice == 'n')
        {
            return 1;
        }
        else
        {
            printf("Invalid input. Enter Y/N: \n");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            scanf(" %c", &choice);
        }
    }
}