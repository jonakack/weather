#include <stdio.h>
#include <curl/curl.h>
#include "../include/weather.h"

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

/* Sets NUMBER_OF_CITIES */
int get_number_of_cities(void) { return sizeof(cities) / sizeof(cities[0]); }

 /* Shows list of available cities */
void cities_showList()
{   
    int i = 0;
    for (i=0; i<get_number_of_cities(); i++)
    {
        printf("[%d] %s\n", cities[i].list, cities[i].name);
    }
}

/* Takes city choice as input and returns list number */
int cities_choice()
{
    int a = 0;
    /* Removed unused variable 'i' - now declared in for loop scope where it's actually used */

    while (1)
    {
        if (scanf("%d", &a) == 1 && a >= 1 && a <= get_number_of_cities())
        {
            break;
        }
        else
        {
            printf("Invalid choice. Try again. \n");
            while (getchar() != '\n'); /* Clear input buffer */
        }
    }

    for (int i = 0; i < get_number_of_cities(); i++)
    {
        if (a == cities[i].list)
        {
            printf("_____________________________________________________________________________________________________\n");
            printf("\nYou chose %s!\n\n", cities[i].name);
            while (getchar() != '\n');
            return a;
        }
    }
    return -1;
}

/* Converts city number to correct URL */
char *makeURL(int cityIndex)
{
    static char url[MAX_URL_LENGTH]; /* Using defined constant instead of magic number */

    sprintf (url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            cities[cityIndex-1].latitude, cities[cityIndex-1].longitude);
    /* Removed buffer clearing from URL generation function - this should be handled by input functions */
    return url;
}

/* Returns 0 if char is Y/y and 1 if N/n */
int askYesNo(char a)
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
    return askYesNo(a); /* Recursive in case of invalid input */
}