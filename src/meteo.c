#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../include/cJSON.h"
#include "../include/meteo.h"
#include "../include/list.h"
#include "../include/cache.h"

char *makeURL(double latitude, double longitude) {
    char *url = malloc(256);
    if (url == NULL) {
        return NULL;
    }
    sprintf(url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            latitude,
            longitude);
    return url;
}

int download_city_data(cityList *city)
{
    printf("Creating URL...\n");
    char *url = makeURL(city->latitude, city->longitude);
    if (url == NULL) {
        return -1;
    }
    printf("Downloading HTTP data...\n");
    char *httpData = http_init(url);
    
    if (httpData == NULL) {
        printf("Failed to download weather data\n");
        free(url);
        return -1;
    }

    printf("Parsing and saving data in %s's file...\n", city->name);
    save_data(city, httpData);
    
    free(url);
    free(httpData);
    return 0;
}

void meteoWeatherCodes(int code, char *desc) {
    static const char *weather[] = {
        [0] = "Clear sky ☀️",
        [1] = "Mainly clear 🌤️",
        [2] = "Partly cloudy ⛅",
        [3] = "Overcast ☁️",
        [45] = "Fog 🌫️",
        [48] = "Depositing rime fog 🌫️",
        [51] = "Light drizzle 🌦️",
        [53] = "Moderate drizzle 🌦️",
        [55] = "Dense drizzle 🌧️",
        [56] = "Light freezing drizzle 🌧️",
        [57] = "Dense freezing drizzle 🌧️",
        [61] = "Slight rain 🌧️",
        [63] = "Moderate rain 🌧️",
        [65] = "Heavy rain ⛈️",
        [66] = "Light freezing rain 🌧️",
        [67] = "Heavy freezing rain 🌧️",
        [71] = "Slight snow fall ❄️",
        [73] = "Moderate snow fall ❄️",
        [75] = "Heavy snow fall ❄️",
        [77] = "Snow grains ❄️",
        [80] = "Slight rain showers 🌦️",
        [81] = "Moderate rain showers 🌧️",
        [82] = "Violent rain showers ⛈️",
        [85] = "Slight snow showers 🌨️",
        [86] = "Heavy snow showers 🌨️",
        [95] = "Thunderstorm ⛈️",
        [96] = "Thunderstorm with slight hail ⛈️",
        [99] = "Thunderstorm with heavy hail ⛈️"
    };

    if (code >= 0 && code < 100 && weather[code] != NULL) {
        sprintf(desc, "%s", weather[code]);
    } else {
        sprintf(desc, "Unknown weather code (%d)", code);
    }
}
