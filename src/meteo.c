#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "../include/libs/cJSON.h"
#include "../include/meteo.h"
#include "../include/cache.h"

char *Meteo_MakeURL(double _Latitude, double _Longitude)
{
    char *url = malloc(256);
    if (url == NULL)
    {
        return NULL;
    }
    sprintf(url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            _Latitude,
            _Longitude);
    return url;
}

void Meteo_ShowWeatherCode(int _WeatherCode, char *_Description)
{
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
        [99] = "Thunderstorm with heavy hail ⛈️"};

    if (_WeatherCode >= 0 && _WeatherCode < 100 && weather[_WeatherCode] != NULL)
    {
        sprintf(_Description, "%s", weather[_WeatherCode]);
    }
    else
    {
        sprintf(_Description, "Unknown weather code (%d)", _WeatherCode);
    }
}
