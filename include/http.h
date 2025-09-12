#include <stdbool.h>

typedef struct
{
    bool temp;
} http;

int http_init(const char *url);

void http_dispose (http* h);