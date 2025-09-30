#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cJSON.h"
#include "../include/json.h"

WeatherData* parse_weather_json(const char *json) {
    // Parse JSON string into cJSON tree structure
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        fprintf(stderr, "JSON parse error\n");
        return NULL;
    }
    
    // Extract the "current_weather" object from JSON
    cJSON *weather_obj = cJSON_GetObjectItemCaseSensitive(root, "current_weather");
    if (!cJSON_IsObject(weather_obj)) {
        cJSON_Delete(root);  // Clean up before returning
        return NULL;
    }
    
    // Allocate memory on heap for WeatherData struct
    WeatherData *weather = malloc(sizeof(WeatherData));
    if (weather == NULL) {
        cJSON_Delete(root);  // Clean up on allocation failure
        return NULL;
    }
    
    // Extract individual weather fields from JSON
    cJSON *temp = cJSON_GetObjectItemCaseSensitive(weather_obj, "temperature");
    cJSON *wind = cJSON_GetObjectItemCaseSensitive(weather_obj, "windspeed");
    cJSON *code = cJSON_GetObjectItemCaseSensitive(weather_obj, "weathercode");
    cJSON *time = cJSON_GetObjectItemCaseSensitive(weather_obj, "time");
    cJSON *winddir = cJSON_GetObjectItemCaseSensitive(weather_obj, "winddirection");
    
    // Populate struct with values, using defaults if data is missing
    weather->temperature = cJSON_IsNumber(temp) ? temp->valuedouble : 0.0;
    weather->windspeed = cJSON_IsNumber(wind) ? wind->valuedouble : 0.0;
    weather->weathercode = cJSON_IsNumber(code) ? code->valueint : 0;
    weather->winddirection = cJSON_IsNumber(winddir) ? winddir->valueint : 0;
    weather->time = cJSON_IsString(time) ? strdup(time->valuestring) : NULL;  // strdup allocates new memory
    
    // Set flags to indicate which fields have valid data
    weather->has_temperature = cJSON_IsNumber(temp);
    weather->has_windspeed = cJSON_IsNumber(wind);
    weather->has_weathercode = cJSON_IsNumber(code);
    weather->has_winddirection = cJSON_IsNumber(winddir);
    weather->has_time = cJSON_IsString(time);
    
    // Clean up cJSON tree (our strdup'd string is safe)
    cJSON_Delete(root);
    return weather;
}

void free_weather_data(WeatherData *weather) {
    if (weather) {
        free(weather->time);  // Free string first
        free(weather);        // Then free struct
    }
}