#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/list.h"
#include "../include/cities.h"
#include "../include/http.h"
#include "../include/cache.h"
#include "../include/meteo.h"

// Convert UTF-8 å, ä, ö to uppercase
static int utf8_swedish_to_upper(const char *src, char *dst)
{
    if ((unsigned char)src[0] == 0xc3)
    {
        if ((unsigned char)src[1] == 0xa5)
        {
            dst[0] = 0xc3;
            dst[1] = 0x85;
            return 2;
        } // å -> Å
        if ((unsigned char)src[1] == 0xa4)
        {
            dst[0] = 0xc3;
            dst[1] = 0x84;
            return 2;
        } // ä -> Ä
        if ((unsigned char)src[1] == 0xb6)
        {
            dst[0] = 0xc3;
            dst[1] = 0x96;
            return 2;
        } // ö -> Ö
        if ((unsigned char)src[1] == 0x85)
        {
            dst[0] = 0xc3;
            dst[1] = 0x85;
            return 2;
        } // Å (already uppercase)
        if ((unsigned char)src[1] == 0x84)
        {
            dst[0] = 0xc3;
            dst[1] = 0x84;
            return 2;
        } // Ä (already uppercase)
        if ((unsigned char)src[1] == 0x96)
        {
            dst[0] = 0xc3;
            dst[1] = 0x96;
            return 2;
        } // Ö (already uppercase)
    }
    // ASCII
    dst[0] = toupper((unsigned char)src[0]);
    return 1;
}

// Convert UTF-8 Å, Ä, Ö to lowercase
static int utf8_swedish_to_lower(const char *src, char *dst)
{
    if ((unsigned char)src[0] == 0xc3)
    {
        if ((unsigned char)src[1] == 0x85)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa5;
            return 2;
        } // Å -> å
        if ((unsigned char)src[1] == 0x84)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa4;
            return 2;
        } // Ä -> ä
        if ((unsigned char)src[1] == 0x96)
        {
            dst[0] = 0xc3;
            dst[1] = 0xb6;
            return 2;
        } // Ö -> ö
        if ((unsigned char)src[1] == 0xa5)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa5;
            return 2;
        } // å (already lowercase)
        if ((unsigned char)src[1] == 0xa4)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa4;
            return 2;
        } // ä (already lowercase)
        if ((unsigned char)src[1] == 0xb6)
        {
            dst[0] = 0xc3;
            dst[1] = 0xb6;
            return 2;
        } // ö (already lowercase)
    }
    dst[0] = tolower((unsigned char)src[0]);
    return 1;
}

void get_city_choice(char *userChoice)
{
    char input[64];
    int len = 0;
    while (scanf("%63s", input) != 1)
    {
        printf("Invalid input\n");
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            // discard characters
        }
    }
    
    // Clear remaining input buffer after successful scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard characters
    }
    
    len = strlen(input);
    int i = 0, j = 0;
    // Uppercase first character (ASCII or UTF-8 Swedish)
    if ((unsigned char)input[i] == 0xc3 && i + 1 < len)
    {
        j += utf8_swedish_to_upper(&input[i], &userChoice[j]);
        i += 2;
    }
    else
    {
        userChoice[j++] = toupper((unsigned char)input[i++]);
    }
    // Lowercase the rest (ASCII or UTF-8 Swedish)
    while (i < len)
    {
        if ((unsigned char)input[i] == 0xc3 && i + 1 < len)
        {
            j += utf8_swedish_to_lower(&input[i], &userChoice[j]);
            i += 2;
        }
        else
        {
            userChoice[j++] = tolower((unsigned char)input[i++]);
        }
    }
    userChoice[j] = '\0';
}

int get_city_data(const char *choice)
{
    cityList *chosenCity = search_list((char *)choice); // Returns a pointer to the city if it is found
    if (chosenCity == NULL)
    {
        printf("City not found.\n");
        return 1;
    }

    // Check if local file exists
    int check_result = check_existing(chosenCity);
    if (check_result == 0 || check_result == -1)
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

    char *httpData = malloc(file_size + 1);
    if (httpData == NULL)
    {
        printf("Failed to allocate memory\n");
        fclose(file);
        return 1;
    }

    fread(httpData, 1, file_size, file);
    httpData[file_size] = '\0';
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