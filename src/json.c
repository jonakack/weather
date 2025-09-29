#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cJSON.h"
#include "../include/json.h"

WeatherData* parse_weather_json(const char *json) { // 
    // Parse JSON string into cJSON tree structure
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        fprintf(stderr, "JSON parse error\n");
        return NULL;
    }
    
    // Extract the "current_weather" object from JSON
    cJSON *weather = cJSON_GetObjectItemCaseSensitive(root, "current_weather");
    if (!cJSON_IsObject(weather)) {
        cJSON_Delete(root);  // Clean up before returning
        return NULL;
    }
    
    // Allocate memory on heap for WeatherData struct
    WeatherData *data = malloc(sizeof(WeatherData));
    if (data == NULL) {
        cJSON_Delete(root);  // Clean up on allocation failure
        return NULL;
    }
    
    // Extract individual weather fields from JSON
    cJSON *temp = cJSON_GetObjectItemCaseSensitive(weather, "temperature");
    cJSON *wind = cJSON_GetObjectItemCaseSensitive(weather, "windspeed");
    cJSON *code = cJSON_GetObjectItemCaseSensitive(weather, "weathercode");
    cJSON *time = cJSON_GetObjectItemCaseSensitive(weather, "time");
    cJSON *winddir = cJSON_GetObjectItemCaseSensitive(weather, "winddirection");
    
    // Populate struct with values, using defaults if data is missing
    data->temperature = cJSON_IsNumber(temp) ? temp->valuedouble : 0.0;
    data->windspeed = cJSON_IsNumber(wind) ? wind->valuedouble : 0.0;
    data->weathercode = cJSON_IsNumber(code) ? code->valueint : 0;
    data->winddirection = cJSON_IsNumber(winddir) ? winddir->valueint : 0;
    data->time = cJSON_IsString(time) ? strdup(time->valuestring) : NULL;  // strdup allocates new memory
    
    // Set flags to indicate which fields have valid data
    data->has_temperature = cJSON_IsNumber(temp);
    data->has_windspeed = cJSON_IsNumber(wind);
    data->has_weathercode = cJSON_IsNumber(code);
    data->has_winddirection = cJSON_IsNumber(winddir);
    data->has_time = cJSON_IsString(time);
    
    // Clean up cJSON tree (our strdup'd string is safe)
    cJSON_Delete(root);
    return data;
}

void free_weather_data(WeatherData *data) {
    if (data) {
        free(data->time);  // Free string first
        free(data);        // Then free struct
    }
}