#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h> // Set compiler path as C:/msys64/ucrt64/bin/gcc.exe on windows

#include "../include/cities.h"
#include "../include/cache.h"
#include "../include/cJSON.h"
#include "../include/list.h"

// Macro to make mkdir work on Windows and Linux
#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0777)
#endif

int check_existing(cityList *city)
{
    DIR *dir = opendir("data");
    if (dir == NULL)
    {
        return ERROR;
    }

    char target_filename[100];
    sprintf(target_filename, "%s_%.4f_%.4f.json", city->name, city->latitude, city->longitude);

    char full_path[200];
    sprintf(full_path, "data/%s", target_filename);

    // Loop through all files in the directory
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, target_filename) == 0)
        {
            closedir(dir);
            if (check_data_age(full_path) == OUT_OF_DATE)
            {
                printf("File exists but is outdated\n");
                return 0; // File exists but is outdated
            }
            else if (check_data_age(full_path) == UP_TO_DATE)
            {
                printf("File exists and is up to date. No action needed\n");
                return -2; // File exists
            }
        }
    }
    closedir(dir);
    printf("File does not exist, creating new one...\n");
    return 0; // File does not exist
}

int save_data(cityList *city, char *httpData)
{
    FILE *fptr = NULL;
    char filename[100];
    int result = -3;

    cJSON *json_obj = cJSON_Parse(httpData);     // Parse data
    char *httpData_json = cJSON_Print(json_obj); // Put parsed JSON data into a string

    // Create folder called "data"
    if (MKDIR("data") != 0 && errno != EEXIST)
    {
        perror("Failed to create directory");
        result = ERROR;
        goto cleanup;
    }

    // Set file name
    sprintf(filename, "data/%s_%.4f_%.4f.json", city->name, city->latitude, city->longitude);
    // Don't overwrite the filename - it's already set correctly in create_city()

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
        result = 0; // Success
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
            return DOES_NOT_EXIST;
        }
        else
            return ERROR;
    }

    time_t mod_time = fileStatus.st_mtime;
    time_t current_time = time(NULL);

    if ((current_time - mod_time) > 900) // Checks if file is older than 15 minutes
    {
        return OUT_OF_DATE;
    }
    else
        return UP_TO_DATE;
}