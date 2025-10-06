#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cJSON.h"
#include "../include/json.h"
#include "../include/meteo.h"

WeatherData *parse_weather_json(const char *json)
{
    cJSON *root = cJSON_Parse(json);
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

    WeatherData *data = malloc(sizeof(WeatherData));
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

void free_weather_data(WeatherData *data)
{
    if (data)
    {
        free(data->time);
        free(data);
    }
}

void display_weather_menu(WeatherData *data, char *cityName)
{
    printf("\n========= Weather in %s =========\n", cityName);

    if (data->has_temperature)
    {
        printf("# Temperature: %.1f °C\n", data->temperature);
    }

    if (data->has_windspeed)
    {
        printf("# Wind Speed: %.1f m/s\n", data->windspeed);
    }

    if (data->has_winddirection)
    {
        printf("# Wind Direction: %d°\n", data->winddirection);
    }

    if (data->has_weathercode)
    {
        char weather_desc[100];
        meteoWeatherCodes(data->weathercode, weather_desc);
        printf("# Weather Conditions: %s\n", weather_desc);
    }

    if (data->has_time)
    {
        printf("# Time: %s\n", data->time);
    }

    printf("=====================================\n\n");
}