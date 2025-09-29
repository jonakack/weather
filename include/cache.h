#ifndef CACHE_H
#define CACHE_H

#define ERROR 1
#define OK 0
#define UP_TO_DATE 0
#define OUT_OF_DATE -1
#define DOES_NOT_EXIST -2

int saveData(char *httpData, int cityIndex);
int check_data_age(const char *filename);
int saveDataHeap(char *httpData, int index);

#endif // CACHE_H