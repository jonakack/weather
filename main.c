#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

int main()
{
    // Allows special characters (å, ä, ö)
    setlocale(LC_CTYPE, "sv_SE.UTF-8");

    const char *list =
    "Stockholm:59.3293:18.0686\n"
    "Göteborg:57.7089:11.9746\n" 
    "Malmö:55.6050:13.0038\n" 
    "Uppsala:59.8586:17.6389\n" 
    "Västerås:59.6099:16.5448\n" 
    "Örebro:59.2741:15.2066\n" 
    "Linköping:58.4109:15.6216\n" 
    "Helsingborg:56.0465:12.6945\n" 
    "Jönköping:57.7815:14.1562\n" 
    "Norrköping:58.5877:16.1924\n" 
    "Lund:55.7047:13.1910\n" 
    "Gävle:60.6749:17.1413\n" 
    "Sundsvall:62.3908:17.3069\n" 
    "Umeå:63.8258:20.2630\n" 
    "Luleå:65.5848:22.1567\n" 
    "Kiruna:67.8558:20.2253\n";

    char buffer[1024];
    strncpy(buffer, list, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    printf("Välkommen!\n");

    printf("Tillgängliga städer: \n");
    char* line = strtok(buffer, "\n");
    while (line != NULL) 
    {
        char city[64];
        float lat, lon;
        sscanf(line, "%[^:]", city);
        printf("%s\n", city);
        line = strtok(NULL, "\n");
    }
    printf("\n\n");

    char choice[20];

    // Repeats program
    while(1)
    {
        printf("Skriv namnet på en stad: \n");
        scanf("%19s", choice);
        choice[0] = toupper(choice[0]);
        for (int i=0; i<strlen(choice); i++)
        {
            choice[i+1] = tolower(choice[i+1]);
        }
        printf("%s\n", choice);

        char buffer2[1024];
        strncpy(buffer2, list, sizeof(buffer2));
        buffer2[sizeof(buffer2) - 1] = '\0';

        char* line2 = strtok(buffer2, "\n");
        int found = 0;
        float lat, lon;

        while (line2 != NULL) {
            char city[64];
            sscanf(line2, "%[^:]:%f:%f", city, &lat, &lon);
            if (strcmp(city, choice) == 0)
            {
                found = 1;
                break;
            }
            line2 = strtok(NULL, "\n");
        }

        const char url_template[200] = "https://api.open-meteo.com/v1/forecast?latitude=%.4f&longitude=%.4f&current_weather=true";
        char url[200];
        sprintf(url, url_template, lat, lon);

        if (!found)
        {
            printf("Staden hittades inte.\n\n");
        }
        else 
        {
            printf("URL: \"%s\"\r\n\n", url);
        }
    }
    return 0;
}