#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#ifdef _WIN32
    #include <direct.h>
    #define stat _stat
#endif
#include "../include/cache.h"
#include "../include/meteo.h"
#include "../include/http.h"
#include "../include/json.h"

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
            printf("Cached data expired %.0f seconds old\n", seconds);
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