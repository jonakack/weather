#include "../include/http.h"
#include "../include/list.h"
#include "../include/meteo.h"

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Memory
{
    char *data; // String that contains data
    size_t size;
};

// this function is called by libcurl when data comes in
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t total = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    // grow buffer to fit new data
    char *ptr = realloc(mem->data, mem->size + total + 1);
    if (ptr == NULL)
    {
        return 0; // out of memory
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total);
    mem->size += total;
    mem->data[mem->size] = '\0'; // null-terminate string

    return total;
}

// This function fetches data from URL and returns it as a string
char *HTTP_Init(const char *_Url)
{
    CURL *curl = curl_easy_init();
    if (!curl)
        return NULL;

    struct Memory chunk = {0};

    curl_easy_setopt(curl, CURLOPT_URL, _Url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (curl_easy_perform(curl) != CURLE_OK)
    {
        curl_easy_cleanup(curl);
        free(chunk.data);
        return NULL;
    }

    curl_easy_cleanup(curl);
    return chunk.data; // caller must free
}

int HTTP_DownloadData(cityList *_City)
{
    printf("Creating URL...\n");
    char *url = Meteo_MakeURL(_City->latitude, _City->longitude);
    if (url == NULL)
    {
        return -1;
    }
    printf("Downloading HTTP data...\n");
    char *httpData = HTTP_Init(url);

    if (httpData == NULL)
    {
        printf("Failed to download weather data\n");
        free(url);
        return -1;
    }

    printf("Parsing and saving data in %s's file...\n", _City->name);
    Cache_SaveData(_City, httpData);

    free(url);
    free(httpData);
    return 0;
}