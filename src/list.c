#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct city
{
    char *name;
    float time;

    struct city *next;
    struct city *prev;
} city_t; 

void add_city_to_list (city_t **head, char* name){
    city_t *city = malloc(sizeof(city_t));
    city->name = name;
 // todo: "city->time = vad nu tiden är"
    city->next = *head;
    city->prev = NULL;

    if (*head != NULL) {
        (*head)->prev = city;
    }
    
    *head = city;
}

void remove_city_from_list(city_t **head, char* name){
    city_t *current = *head;
    while (current != NULL) 
    {
        if (strcmp(current->name, name) == 0){ /* Break loop if they match */
            current->prev->next = current->next;
            printf("Removed %s\n", name);
            break;
        } else current = current->next;
    } 
    if (current == NULL)
    {
        printf("City not found\n");
    }
    
}

void show_list(city_t *head){
    city_t *current = head;
    while(current != NULL)
    {
        printf("%s\n", current->name);
        current = current->next;
    }
    current = head; /* Resets *current to starting position */
}


//Tvungen att kommentera ut tillsvidare för att kunna köra make run.
/* int main()
{
    city_t *head = NULL;

    add_city_to_list(&head, "Stockholm");
    add_city_to_list(&head, "Linköping");
    add_city_to_list(&head, "Norrköping");
    add_city_to_list(&head, "Göteborg");
    add_city_to_list(&head, "Umeå");

    remove_city_from_list(&head, "Norrköping");

    show_list(head);

    return 0;
} */