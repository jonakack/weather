#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#include "../include/cities.h"
#include "../include/cache.h"
#include "../include/cJSON.h"

// Macro to make mkdir work on Windows and Linux
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

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

int check_data_age(const char *filename)
{
    struct stat fileStatus;

    if (stat(filename, &fileStatus) != OK)
    {
        if (errno == ENOENT) // errno checks if error is because file doesn't exist
        {
            printf("File does not exist, creating new one... \n");
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
        printf("File is older than 15 minutes, creating new one... \n");
        return OUT_OF_DATE;
    }
    else
    {
        printf("File is up to date. No new file needed. \n");
        return UP_TO_DATE;
    }
}