#ifndef CACHE_H
#define CACHE_H

#define OK 0
#define ERROR 1
#define UP_TO_DATE 0
#define OUT_OF_DATE -1
#define DOES_NOT_EXIST -2

int check_data_age(const char *filename);

#endif // CACHE_H