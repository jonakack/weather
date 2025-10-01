#include <stdio.h>
#include <ctype.h>
#include "../include/cJSON.h"
#include "../include/meteo.h"
#include "../include/city.h"

// OLD VERSION (using cities[] array)
/*
void makeURL(int cityIndex, char *url) {
    sprintf (url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            cities[cityIndex-1].latitude,
            cities[cityIndex-1].longitude);
    while (getchar() != '\n'); // clear the input buffer so \n doesn't follow into the next user input
}
*/

// Updated version using CityData struct
void makeURLFromCity(CityData* city, char *url) {
    sprintf(url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            city->latitude, city->longitude);
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
