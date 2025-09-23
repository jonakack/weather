#ifndef CACHE_H
#define CACHE_H

void read_or_create_file(const char *url, int cityIndex);
void cleanup_old_weather_files();
#endif