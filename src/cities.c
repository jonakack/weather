#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h>
    #define stat _stat
#endif
#include "../include/weather.h"
#include "../include/http.h"
#include "../include/json.h"

const char *cities_str = "Stockholm:59.3293:18.0686\n" "Göteborg:57.7089:11.9746\n" 
    "Malmö:55.6050:13.0038\n" "Uppsala:59.8586:17.6389\n" "Västerås:59.6099:16.5448\n" 
    "Örebro:59.2741:15.2066\n" "Linköping:58.4109:15.6216\n" "Helsingborg:56.0465:12.6945\n" 
    "Jönköping:57.7815:14.1562\n" "Norrköping:58.5877:16.1924\n" "Lund:55.7047:13.1910\n" 
    "Gävle:60.6749:17.1413\n" "Sundsvall:62.3908:17.3069\n" "Umeå:63.8258:20.2630\n" 
    "Luleå:65.5848:22.1567\n" "Kiruna:67.8558:20.2253\n" "Skellefteå:64.7506:20.9528\n";

// Global variables used in multiple functions in cities.c
struct city *cities = NULL;
int city_count;


// Instead of manually reformatting the string "cities_str" into a struct
// this function will take the string and create a struct array in the right format
// this is to account for further expansion: what if the list contains 10 000 cities?
void build_citystruct() {

    city_count = 0;
    // Count how many cities are in the string
    for (const char *p = cities_str; *p;) { // Create a pointer *p at the beginning of cities_str; *p -> Looks at the current character
        if (*p == '\n') city_count++;       // if (*p == '\n') city_count++ -> if current character "*p" is looking at "\n" increase city_count by 1. Since each city ends with \n this is fine
        p++;                                // Move the pointer to the next character. This for loop will end when *p is currently looking at \0 aka NULL terminator
    }                                          

    // Allocate memory for the city struct array
    cities = malloc(city_count * sizeof(struct city)); // this allocated memory is currently being freed in main.c after we are done using it
    if (cities == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }
    
    // Create a buffer to copy "cities_str" and also allocate enough memory for it
    // strtok modifies the original string, we need a buffer because cities_str is a const char == can't be modified
    char *buffer = malloc(strlen(cities_str) + 1);  // Declare a pointer "buffer" that can point to a char, and also allocate enough bytes on the heap for it to copy cities_str
    if (buffer == NULL) {                           // ^ +1 is to account for \0
        printf("Failed to allocate memory for buffer\n");
        free(cities);
        exit(1);
    }
    strcpy(buffer, cities_str);                     // Copy the contents from cities_str into buffer

    // Parse the string and build the struct array "cities"
    char *line = strtok(buffer, "\n");      // strtok splits the string into tokens based on the delimiter "\n"
    int index = 0;
    while (line && index < city_count) {    // while line is not NULL and index is less than city_count
        sscanf(line, "%49[^:]:%lf:%lf",     // %49[^:] means read up to 49 characters or until ':' is encountered.
            cities[index].name,
            &cities[index].latitude, 
            &cities[index].longitude);
        index++;
        line = strtok(NULL, "\n");          
    }
    free(buffer);   // Free the allocated memory for buffer
    // We don't free the allocated memory for cities yet, we still need it in other functions
}

 /* Shows list of available cities */
void cities_showList() {   
    int i = 0;
    for (i=0; i<city_count; i++)
    {
        printf("[%d] %s\n", i+1, cities[i].name);
    }
}

/* Takes city choice as input and returns list number */
int cities_choice() {   
    int a = 0;
    int i = 0;
    
    while (1) {
        if (scanf("%d", &a) == 1 && a >= 1 && a <= city_count) {
            break;
        }
        else {
            printf("Invalid choice. Try again. \n");
            while (getchar() != '\n'); // clear the input buffer, can cause infinite loop otherwise
        }
    }

    for (i=0; i<city_count; i++)
    {
        if (a == i+1) // Changed from a == cities[i].list to a == i+1 because I removed the list variable from the struct
        {
            printf("_____________________________________________________________________________________________________\n");
            printf("\nYou chose %s!\n\n", cities[i].name);
            
            return a;
        }
    }
    return -1;
}



/* Converts city number to correct URL */
void makeURL(int cityIndex, char *url) {   // Changed function signature to take char *url as parameter to write directly into it

    sprintf (url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true", 
            cities[cityIndex-1].latitude, 
            cities[cityIndex-1].longitude);
    while (getchar() != '\n'); // clear the input buffer so \n doesn't follow into the next user input
    //printf("%s\n", url);  // !!!DEBUGGING!!!
}



/* Returns 0 if char is Y/y and 1 if N/n */
int askYesNo(char a)
{
    if  (a == 'Y' || a == 'y') {
        return 0;
    }  
    else if (a == 'N' || a == 'n'){
        return 1;
    }
    else printf("Invalid input. Enter Y/N: \n"); 
    scanf(" %c", &a);
    return askYesNo(a); /* Recursive in case of invalid input */
}

void readorcreatefile(const char *url, int cityIndex) {
    char filename[256];
    const char *folder = "weather_data";

    snprintf(filename, sizeof(filename), "%s/%s.txt", folder, cities[cityIndex - 1].name); //Build the path to the file

    struct stat file_info; // Creates a variable called file_info that holds metadata about the file such as time of last modification
    FILE *city_file = fopen(filename, "r"); // Try to open the file for reading
    char *json_str = NULL;
    

    if (stat(filename, &file_info) == 0) { // If the file exists
        time_t current_time = time(NULL); // Get the current time
        double seconds = difftime(current_time, file_info.st_mtime); // Calculate the difference in seconds between current time and file's last modification time

        if (seconds <= 15 * 60) { // If the last modification is less than 900 seconds old, read from it
            city_file = fopen(filename, "r");
            if (city_file) {
                fseek(city_file, 0, SEEK_END); // Move the file pointer to the end of the file
                long size = ftell(city_file);  // Get the current position of the file pointer save it to variable file
                rewind(city_file); 
                
                json_str = malloc(size + 1); // Allocate memory to hold the file contents +1 for \0
                if (json_str) {
                    fread(json_str, 1, size, city_file); 
                    json_str[size] = '\0'; // Null-terminate the string
                }
            fclose(city_file);
            printf("Succesfully loaded cached data! The data is %.0f seconds old\n", seconds);
            }
        
    } else { 
        printf("Cached data expired %.0f seconds olds\n", seconds);
        }
    }
    if (json_str == NULL) {
        json_str = http_init(url); // Fetch data from the API if the file doesn't exist
        if (json_str) {
            city_file = fopen(filename, "w");
            if (city_file) {
                fputs(json_str, city_file);
                fclose(city_file);
                printf("Fetched data and saved it to %s.txt\n", cities[cityIndex - 1].name);
                }
            } else {
                fprintf(stderr, "Could not open file %s for writing\n", filename);
            }    
        }
    
    if (json_str) {
        parse_weather_json(json_str); // anropa funktionen för att parsa JSON-datan 
        free(json_str); // frigör minnet som allokerats för JSON-strängen
    } else {
        printf("Failed to get or load data\n");
    }   
}



