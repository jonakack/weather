#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libs/cJSON.h"
#include "../include/json.h"
#include "../include/meteo.h"

JSON_WeatherData *JSON_ParseData(const char *_Json)
{
    cJSON *root = cJSON_Parse(_Json);
    if (root == NULL)
    {
        fprintf(stderr, "JSON parse error\n");
        return NULL;
    }

    cJSON *weather = cJSON_GetObjectItemCaseSensitive(root, "current_weather");
    if (!cJSON_IsObject(weather))
    {
        cJSON_Delete(root);
        return NULL;
    }

    JSON_WeatherData *data = malloc(sizeof(JSON_WeatherData));
    if (data == NULL)
    {
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

void JSON_FreeWeatherData(JSON_WeatherData *_Data)
{
    if (_Data)
    {
        free(_Data->time);
        free(_Data);
    }
}

void JSON_PrintWeatherData(JSON_WeatherData *_Data, char *_CityName)
{
    printf("\n========= Weather in %s =========\n", _CityName);

    if (_Data->has_temperature)
    {
        printf("# Temperature: %.1f °C\n", _Data->temperature);
    }

    if (_Data->has_windspeed)
    {
        printf("# Wind Speed: %.1f m/s\n", _Data->windspeed);
    }

    if (_Data->has_winddirection)
    {
        printf("# Wind Direction: %d°\n", _Data->winddirection);
    }

    if (_Data->has_weathercode)
    {
        char weather_desc[100];
        Meteo_ShowWeatherCode(_Data->weathercode, weather_desc);
        printf("# Weather Conditions: %s\n", weather_desc);
    }

    if (_Data->has_time)
    {
        printf("# Time: %s\n", _Data->time);
    }

    printf("=====================================\n\n");
}