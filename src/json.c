#include <stdio.h>
#include <cjson/cJSON.h>
#include <ctype.h>
#include "../include/json.h"


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

        while (1) {
            printf("Vad vill du veta? (t)emperatur, (v)indhastighet, väder(k)od, t(i)d, vind(r)iktning eller (n) för att avbryta: ");
            char line[16];
            if (!fgets(line, sizeof(line), stdin)) break;
            char choice = tolower(line[0]);
            if (choice == 't' && cJSON_IsNumber(temp))
                printf("Temperature: %.1f °C\n", temp->valuedouble);
            else if (choice == 'v' && cJSON_IsNumber(wind))
                printf("Windspeed: %.1f m/s\n", wind->valuedouble);
            else if (choice == 'k' && cJSON_IsNumber(code))
                printf("Weather code: %d\n", code->valueint);
            else if (choice == 'i' && cJSON_IsString(time))
                printf("Time: %s\n", time->valuestring);
            else if (choice == 'r' && cJSON_IsNumber(winddir))
                printf("Wind direction: %d°\n", winddir->valueint);
            else if (choice == 'n')
                break;
            else
                printf("Ogiltigt val eller data saknas.\n");
        }
        
    }

    cJSON_Delete(root);
}