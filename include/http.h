#ifndef HTTP_H
#define HTTP_H

#include "list.h"

char* HTTP_Init(const char* _Url);
int HTTP_DownloadData(cityList *_City);

#endif // HTTP_H