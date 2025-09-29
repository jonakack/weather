#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cJSON.h"
#include "../include/json.h"

WeatherData* parse_weather_json(const char *json) {
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        fprintf(stderr, "JSON parse error\n");
        return NULL;
    }
    
    cJSON *weather = cJSON_GetObjectItemCaseSensitive(root, "current_weather");
    if (!cJSON_IsObject(weather)) {
        cJSON_Delete(root);
        return NULL;
    }
    
    WeatherData *data = malloc(sizeof(WeatherData));
    if (data == NULL) {
        cJSON_Delete(root);
        return NULL;
    }
    
    cJSON *temp = cJSON_GetObjectItemCaseSensitive(weather, "temperature");
    cJSON *wind = cJSON_GetObjectItemCaseSensitive(weather, "windspeed");
    cJSON *code = cJSON_GetObjectItemCaseSensitive(weather, "weathercode");
    cJSON *time = cJSON_GetObjectItemCaseSensitive(weather, "time");
    cJSON *winddir = cJSON_GetObjectItemCaseSensitive(weather, "winddirection");
    
    data->temperature = cJSON_IsNumber(temp) ? temp->valuedouble : 0.0;
    data->windspeed = cJSON_IsNumber(wind) ? wind->valuedouble : 0.0;
    data->weathercode = cJSON_IsNumber(code) ? code->valueint : 0;
    data->winddirection = cJSON_IsNumber(winddir) ? winddir->valueint : 0;
    data->time = cJSON_IsString(time) ? strdup(time->valuestring) : NULL;
    
    data->has_temperature = cJSON_IsNumber(temp);
    data->has_windspeed = cJSON_IsNumber(wind);
    data->has_weathercode = cJSON_IsNumber(code);
    data->has_winddirection = cJSON_IsNumber(winddir);
    data->has_time = cJSON_IsString(time);
    
    cJSON_Delete(root);
    return data;
}

void free_weather_data(WeatherData *data) {
    if (data) {
        free(data->time);
        free(data);
    }
}