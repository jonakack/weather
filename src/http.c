#include <stdio.h>
#include <curl/curl.h>
#include "../include/http.h"

int http_init(const char *url)
{
    CURL *curl = curl_easy_init();
    if (curl == NULL)
    {
        printf("Initialization failed\n");
        return -1;
    }

    /* set options */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); */

    /* perform out action */
    CURLcode result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        printf("download problem: %s\n", curl_easy_strerror(result));
        curl_easy_cleanup(curl);
        return -2;
    }

    curl_easy_cleanup(curl);
    return 0;
}