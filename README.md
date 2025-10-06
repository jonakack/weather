# WeatherApp

WeatherApp is a command-line application written in C that allows users to view current weather information for Swedish cities. The app fetches weather data from the [Open-Meteo API](https://open-meteo.com/), caches results locally, and supports UTF-8 city names (including Swedish characters).

## Features

- Select from a list of major Swedish cities.
- Fetches current weather (temperature, wind, weather code, etc.) from Open-Meteo.
- Caches weather data locally for 15 minutes to reduce API calls.
- Handles UTF-8 input for Swedish city names.
- Simple, interactive terminal interface.

## Build Instructions

### Prerequisites

- GCC or compatible C compiler
- [libcurl](https://curl.se/libcurl/) (for HTTP requests)
- [cJSON](https://github.com/DaveGamble/cJSON) (included in `include/libs/`)
- Make

### Build

make

### Run

make run

### Usage

1. Run the application.
2. Select a city by typing its name (case-insensitive, Swedish letters supported).
3. View the current weather information.
4. Choose to select another city or exit.