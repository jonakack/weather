#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#include "../include/cities.h"
#include "../include/cache.h"
#include "../include/cJSON.h"
#include "../include/city.h"
#include "../include/json.h"

// Macro to make mkdir work on Windows and Linux
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

// Old version - USING cities[] array
/*
int saveDataHeap(char *httpData, int index)
{
    cities[index - 1].content = malloc(strlen(httpData) + 1);
    if (cities[index - 1].content == NULL)
    {
        return 1;
    }
    strcpy(cities[index - 1].content, httpData);

    cities[index - 1].savedOrNot = true;
    return 0;
}

int saveData(char *httpData, int index)
{
    FILE *fptr = NULL;
    char filename[100];
    int result = -3;

    cJSON *json_obj = cJSON_Parse(httpData);     // Parse data
    char *httpData_json = cJSON_Print(json_obj); // Put parsed JSON data into a string

    // Create folder called "data"
    if (MKDIR("data") != OK)
    {
        if (errno == EEXIST)
        {
            // Directory exists, this is OK
        }
        else
        {
            perror("Failed to create directory");
            result = ERROR;
            goto cleanup;
        }
    }

    // Set file name
    sprintf(filename, "data/%s_%.4f_%.4f.json", cities[index - 1].name, cities[index - 1].latitude, cities[index - 1].longitude);

    int fileStatusResult = check_data_age(filename);
    if (fileStatusResult == OUT_OF_DATE || fileStatusResult == DOES_NOT_EXIST)
    {
        // Create file in data folder
        if ((fptr = fopen(filename, "w")) == NULL)
        {
            perror("Failed to open file");
            result = ERROR;
            goto cleanup;
        }
        else
        {
            if (fprintf(fptr, "%s", httpData_json) < OK)
            {
                perror("Failed to write to file");
                result = ERROR;
                goto cleanup;
            }
            if (fclose(fptr) != OK)
            {
                perror("Failed to close file");
                result = ERROR;
                goto cleanup;
            }
            fptr = NULL;
        }
        result = OUT_OF_DATE; // No new file needed
        goto cleanup;
    }

cleanup:
    if (fptr != NULL)
    {
        fclose(fptr);
    }
    free(httpData_json);
    cJSON_Delete(json_obj);
    return result;
}
*/

// Updated code for saveing weather data using CityData struct (instead of cityIndex)
int saveWeatherData(CityData* city, const char* httpData)
{
    if(city == NULL || httpData == NULL)
        return -1;

    FILE *fptr = NULL;
    char filename[256];
    int result = -3;

    cJSON *json_obj = cJSON_Parse(httpData);     // Parse data
    char *httpData_json = cJSON_Print(json_obj); // Put parsed JSON data into a string

    // Create folder called "data"
    if (MKDIR("data") != OK)
    {
        if (errno == EEXIST)
        {
            // Directory exists, this is OK
        }
        else
        {
            perror("Failed to create directory");
            result = ERROR;
            goto cleanup;
        }
    }

    // Set file name using CityData
    snprintf(filename, sizeof(filename), "data/%s_%.2f_%.2f.json",
             city->name, city->latitude, city->longitude);

    // Check file status without printing messages
    struct stat fileStatus;
    int file_exists = (stat(filename, &fileStatus) == OK);

    // Always write new data (overwrite if exists)
    if ((fptr = fopen(filename, "w")) == NULL)
    {
        perror("Failed to open file");
        result = ERROR;
        goto cleanup;
    }

    if (fprintf(fptr, "%s", httpData_json) < OK)
    {
        perror("Failed to write to file");
        result = ERROR;
        goto cleanup;
    }

    if (fclose(fptr) != OK)
    {
        perror("Failed to close file");
        result = ERROR;
        goto cleanup;
    }

    fptr = NULL;
    result = OK;

    // Print single message only
    if(!file_exists)
        printf("Cached weather data for %s\n", city->name);
    else
        printf("Updated cache for %s\n", city->name);

cleanup:
    if (fptr != NULL)
    {
        fclose(fptr);
    }
    free(httpData_json);
    cJSON_Delete(json_obj);
    return result;
}

// Updated for loading cached weather data if it exists and is up to date (for a specific city)
char* loadWeatherData(CityData* city)
{
    if(city == NULL)
        return NULL;

    char filename[256];
    snprintf(filename, sizeof(filename), "data/%s_%.2f_%.2f.json",
             city->name, city->latitude, city->longitude);

    // Check if file exists and is up to date
    int fileStatus = check_data_age(filename);
    if(fileStatus != UP_TO_DATE)
    {
        return NULL; // File doesn't exist or is out of date
    }

    // Read file contents
    FILE *fp = fopen(filename, "r");
    if(fp == NULL)
        return NULL;

    // Get file size
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate buffer
    char *buffer = malloc(file_size + 1);
    if(buffer == NULL)
    {
        fclose(fp);
        return NULL;
    }

    // Read file
    size_t bytes_read = fread(buffer, 1, file_size, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);

    printf("Using cached weather data for %s\n", city->name);
    return buffer;
}

int check_data_age(const char *filename)
{
    struct stat fileStatus;

    if (stat(filename, &fileStatus) != OK)
    {
        if (errno == ENOENT) // errno checks if error is because file doesn't exist
        {
            // printf("File does not exist, creating new one... \n"); // Removed to avoid duplicate messages
            return DOES_NOT_EXIST;
        }
        else
        {
            perror("Failed to get file status");
            return ERROR;
        }
    }

    time_t mod_time = fileStatus.st_mtime;
    time_t current_time = time(NULL);

    if ((current_time - mod_time) > 900) // Checks if file is older than 15 minutes
    {
        // printf("File is older than 15 minutes, creating new one... \n"); // Removed to avoid duplicate messages
        return OUT_OF_DATE;
    }
    else
    {
        // printf("File is up to date. No new file needed. \n"); // Removed to avoid duplicate messages
        return UP_TO_DATE;
    }
}