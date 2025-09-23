#include <stdio.h>
#include <cjson/cJSON.h>
#include <ctype.h>
#include "../include/meteo.h"



void makeURL(int cityIndex, char *url) {
    sprintf (url, "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true",
            cities[cityIndex-1].latitude,
            cities[cityIndex-1].longitude);
    while (getchar() != '\n'); // clear the input buffer so \n doesn't follow into the next user input
}