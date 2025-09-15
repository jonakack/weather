#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// struct to hold the downloaded data
struct Memory {
    char *data;
    size_t size;
};

// this function is called by libcurl when data comes in
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    // grow buffer to fit new data
    char *ptr = realloc(mem->data, mem->size + total + 1);
    if (ptr == NULL) {
        return 0; // out of memory
    }

    mem->data = ptr;
    memcpy(&(mem->data[mem->size]), contents, total);
    mem->size += total;
    mem->data[mem->size] = '\0'; // null-terminate string

    return total;
}

// this function fetches data from URL and returns it as a string
char* http_init(const char* url) {
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;

    struct Memory chunk = {0};

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (curl_easy_perform(curl) != CURLE_OK) {
        curl_easy_cleanup(curl);
        free(chunk.data);
        return NULL;
    }

    curl_easy_cleanup(curl);
    return chunk.data; // caller must free
}
