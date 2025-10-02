#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/list.h"

char *stringList = "Stockholm:59.3293:18.0686\n"
                   "Göteborg:57.7089:11.9746\n"
                   "Malmö:55.6050:13.0038\n"
                   "Uppsala:59.8586:17.6389\n"
                   "Västerås:59.6099:16.5448\n"
                   "Örebro:59.2741:15.2066\n"
                   "Linköping:58.4109:15.6216\n"
                   "Helsingborg:56.0465:12.6945\n"
                   "Jönköping:57.7815:14.1562\n"
                   "Norrköping:58.5877:16.1924\n"
                   "Lund:55.7047:13.1910\n"
                   "Gävle:60.6749:17.1413\n"
                   "Sundsvall:62.3908:17.3069\n"
                   "Umeå:63.8258:20.2630\n"
                   "Luleå:65.5848:22.1567\n"
                   "Kiruna:67.8558:20.2253\n"
                   "Skellefteå:64.7506:20.9528\n";

cityList *head;

int load_city_list()
{
    build_cities_list(stringList);
    return 0;
}

void build_cities_list(char *stringList) // Takes a string of cities, separates them
{
    char *list_copy = strdup(stringList);
    if (list_copy == NULL)
    {
        printf("Failed to allocate memory for list copy\n");
        return;
    }

    char *ptr = list_copy;

    char *name = NULL;
    char *lat_str = NULL;
    char *lon_str = NULL;

    do
    {
        if (name == NULL)
        {
            name = ptr;
        }
        else if (lat_str == NULL)
        {
            if (*(ptr) == ':')
            {
                lat_str = ptr + 1;
                *(ptr) = '\0';
            }
        }
        else if (lon_str == NULL)
        {
            if (*(ptr) == ':')
            {
                lon_str = ptr + 1;
                *(ptr) = '\0';
            }
        }
        else
        {
            if (*(ptr) == '\n')
            {
                *(ptr) = '\0';

                // printf("City: <%s>, Latitude: <%s>, Longitude: <%s>\n", name, lat_str, lon_str);

                create_city(name, lat_str, lon_str);

                name = NULL;
                lat_str = NULL;
                lon_str = NULL;
            }
        }

        ptr++;

    } while (*(ptr) != '\0');

    free(list_copy);
}

cityList *initialize_city() // Creates an empty city and returns its adress
{
    cityList *initialize_city = malloc(sizeof(cityList));
    if (initialize_city == NULL)
    {
        return NULL;
    }
    // Initialize all fields to safe values
    initialize_city->name = NULL;
    initialize_city->latitude = 0.0;
    initialize_city->longitude = 0.0;
    initialize_city->filename = NULL;
    initialize_city->next = NULL;
    initialize_city->prev = NULL;

    return initialize_city;
}

void create_city(char *name, char *lat, char *lon)
{
    cityList *new_city = initialize_city();
    if (new_city == NULL)
    {
        printf("Failed to allocate memory for city\n");
        return;
    }

    new_city->name = strdup(name);
    new_city->latitude = atof(lat);
    new_city->longitude = atof(lon);

    // Create filename
    char filename[200];
    sprintf(filename, "data/%s_%.4f_%.4f.json", name, atof(lat), atof(lon));
    new_city->filename = strdup(filename);

    // Check for allocation failures
    if (new_city->name == NULL || new_city->filename == NULL)
    {
        printf("Failed to allocate memory for city strings\n");
        free(new_city->name);
        free(new_city->filename);
        free(new_city);
        return;
    }

    new_city->next = NULL;
    new_city->prev = NULL;

    add_city_to_list(new_city);
}

int add_city_to_list(cityList *city)
{
    if (head == NULL)
    {
        head = city;
    }
    else
    {
        city->next = head;
        head->prev = city;
        head = city;
    }
    return 0;
}

void remove_city_from_list(cityList **head, char *name)
{
    cityList *current = *head;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        { /* Break loop if they match */
            if (current->prev != NULL)
            {
                current->prev->next = current->next;
            }
            else
            {
                // Removing head node
                *head = current->next;
            }
            if (current->next != NULL)
            {
                current->next->prev = current->prev;
            }
            free(current->name);
            free(current->filename);
            free(current);
            printf("Removed %s\n", name);
            break;
        }
        else
            current = current->next;
    }
    if (current == NULL)
    {
        printf("City not found\n");
    }
}

void show_cities_list()
{
    cityList *current = head;
    while (current != NULL)
    {
        printf("%s\n", current->name);
        current = current->next;
    }
    current = head; /* Resets *current to starting position */
}

cityList *search_list(char *target)
{
    cityList *current = head;
    while (current != NULL)
    {
        if ((strcmp(target, current->name)) == 0)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}