#ifndef JSON_H
#define JSON_H

typedef struct {
    double temperature;
    double windspeed;
    int weathercode;
    int winddirection;
    char *time;
    int has_temperature;
    int has_windspeed;
    int has_weathercode;
    int has_winddirection;
    int has_time;
} JSON_WeatherData;

JSON_WeatherData* JSON_ParseData(const char *_Json);

void JSON_PrintWeatherData(JSON_WeatherData *_Data, char *_CityName);
void JSON_FreeWeatherData(JSON_WeatherData *_Data);

#endif