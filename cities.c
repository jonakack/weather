#include <stdio.h>
#include "weather.h"

city cities[] = {
    {1, "Stockholm",    59.3293, 18.0686},
    {2, "Göteborg",     57.7089, 11.9746},
    {3, "Malmö",        55.6050, 13.0038},
    {4, "Uppsala",      59.8586, 17.6389},
    {5, "Västerås",     59.6099, 16.5448},
    {6, "Örebro",       59.2741, 15.2066},
    {7, "Linköping",    58.4109, 15.6216},
    {8, "Helsingborg",  56.0465, 12.6945},
    {9,  "Jönköping",   57.7815, 14.1562},
    {10, "Norrköping",  58.5877, 16.1924},
    {11, "Lund",        55.7047, 13.1910},
    {12, "Gävle",       60.6749, 17.1413},
    {13, "Sundsvall",   62.3908, 17.3069},
    {14, "Umeå",        63.8258, 20.2630},
    {15, "Luleå",       65.5848, 22.1567},
    {16, "Kiruna",      67.8558, 20.2253}
};

const int NUMBER_OF_CITIES = sizeof(cities) / sizeof(cities[0]); // Sets NUMBER_OF_CITIES

// Shows list of available cities
void cities_showList()
{
    for (int i=0; i<NUMBER_OF_CITIES; i++)
    {
        printf("[%d] %s\n", cities[i].list, cities[i].name);
    }
}

// Takes city choice as input and returns list number
int cities_choice()
{   
    int a = 0;
    
    scanf("%d", &a);
    while (a < 1 || a > NUMBER_OF_CITIES)
    {
        printf("Invalid choice. Try again. \n");
        scanf("%d", &a);
    }

    for (int i=0; i<NUMBER_OF_CITIES; i++)
    {
        if (a == cities[i].list)
        {
            printf("_____________________________________________________________________________________________________\n");
            printf("\nYou chose %s!\n", cities[i].name);
            return a;
        }
    }
    return -1;
}

void printURL(int a)
{
    char url[256];
    sprintf (url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true", 
            cities[a-1].latitude, cities[a-1].longitude);

    printf("URL: \"%s\"\r\n", url);
    printf("_____________________________________________________________________________________________________\n\n");
}

// Returns 0 if char is Y/y and 1 if N/n
int selection(char a)
{
    if  (a == 'Y' || a == 'y')
    {
        return 0;
    }  
    else if (a == 'N' || a == 'n')
    {
        return 1;
    }
    else printf("Invalid input. Enter Y/N: \n"); 
    scanf(" %c", &a);
    return selection(a); // Recursive in case of invalid input
}