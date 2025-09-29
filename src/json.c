#include <stdio.h>
#include <ctype.h>
#include "../include/cJSON.h"
#include "../include/json.h"
#include "../include/meteo.h"

void parse_weather_json(const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        fprintf(stderr, "JSON parse error\n");
        return;
    }
    cJSON *weather = cJSON_GetObjectItemCaseSensitive(root, "current_weather");
    if (cJSON_IsObject(weather)) {
        cJSON *temp = cJSON_GetObjectItemCaseSensitive(weather, "temperature");
        cJSON *wind = cJSON_GetObjectItemCaseSensitive(weather, "windspeed");
        cJSON *code = cJSON_GetObjectItemCaseSensitive(weather, "weathercode");
        cJSON *time = cJSON_GetObjectItemCaseSensitive(weather, "time");
        cJSON *winddir = cJSON_GetObjectItemCaseSensitive(weather, "winddirection");
        printf("What would you like to know? \n"
               "[1] Temperature \n"
               "[2] WindSpeed \n"
               "[3] Weather conditions \n"
               "[4] Time \n"
               "[5] Wind direction \n"
               "(n) To cancel\n");
        while (1) {
            char line[16];
            if (!fgets(line, sizeof(line), stdin)) break;
            char choice = tolower(line[0]);
            if (choice == '1' && cJSON_IsNumber(temp)) {
                printf("Temperature: %.1f °C\n", temp->valuedouble);
            }
            else if (choice == '2' && cJSON_IsNumber(wind)) {
                printf("Windspeed: %.1f m/s\n", wind->valuedouble);
            }
            else if (choice == '3' && cJSON_IsNumber(code)) {
                char weather_desc[100];
                meteoWeatherCodes(code->valueint, weather_desc);
                printf("Weather conditions: %s\n", weather_desc, code->valueint);
            }
            else if (choice == '4' && cJSON_IsString(time)) {
                printf("Time: %s\n", time->valuestring);
            }
            else if (choice == '5' && cJSON_IsNumber(winddir)) {
                printf("Wind direction: %d°\n", winddir->valueint);
            }
            else if (choice == 'n') {
                break;
            }
            else {
                printf("Incorrect choice or data missing.\n");
            }
        }
    }
    cJSON_Delete(root);
}