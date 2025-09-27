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

int saveData(char *httpData, int i)
{
    FILE *fptr;
    char filename[100];

    cJSON *json_obj = cJSON_Parse(httpData);     // Parse data
    char *httpData_json = cJSON_Print(json_obj); // Put parsed JSON data into a string

    // free(httpData_json);
    // cJSON_Delete(json_obj);

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
            return ERROR;
        }
    }

    // Set file name
    sprintf(filename, "data/%s_%.4f_%.4f.json", cities[i - 1].name, cities[i - 1].latitude, cities[i - 1].longitude);

    if (check_data_age(filename) == OUT_OF_DATE || DOES_NOT_EXIST)
    {
        // Create file in data folder
        if ((fptr = fopen(filename, "w")) == NULL)
        {
            perror("Failed to open file");
            return ERROR;
        }
        else
        {
            if (fprintf(fptr, "%s", httpData_json) < OK)
            {
                perror("Failed to write to file");
                return ERROR;
            }
            if (fclose(fptr) != OK)
            {
                perror("Failed to close file");
                return ERROR;
            }
        }
        return OUT_OF_DATE; // Made a new file
    }
    return OK; // No new file needed
}

int check_data_age(const char *filename)
{
    struct stat fileStatus;

    if (stat(filename, &fileStatus) == OK)
    {
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
    else
        printf("File does not exist, creating new one... \n");
    return DOES_NOT_EXIST; // Create a new one
}