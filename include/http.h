#ifndef HTTP_H
#define HTTP_H

#include "list.h"

char* HTTP_Init(const char* url);
int HTTP_DownloadData(cityList *city);

#endif // HTTP_H