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
} WeatherData;

// Parses current weather json and returns a WeatherData struct
WeatherData* parse_weather_json(const char *json);

// Free the allocated WeatherData struct
void free_weather_data(WeatherData *data);

#endif