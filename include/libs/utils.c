#include <ctype.h>
#include <stdio.h>
#include "utils.h"
// Convert UTF-8 å, ä, ö to uppercase
int UTF8_SwedishToUpper(const char *src, char *dst)
{
    if ((unsigned char)src[0] == 0xc3)
    {
        if ((unsigned char)src[1] == 0xa5)
        {
            dst[0] = 0xc3;
            dst[1] = 0x85;
            return 2;
        } // å -> Å
        if ((unsigned char)src[1] == 0xa4)
        {
            dst[0] = 0xc3;
            dst[1] = 0x84;
            return 2;
        } // ä -> Ä
        if ((unsigned char)src[1] == 0xb6)
        {
            dst[0] = 0xc3;
            dst[1] = 0x96;
            return 2;
        } // ö -> Ö
        if ((unsigned char)src[1] == 0x85)
        {
            dst[0] = 0xc3;
            dst[1] = 0x85;
            return 2;
        } // Å (already uppercase)
        if ((unsigned char)src[1] == 0x84)
        {
            dst[0] = 0xc3;
            dst[1] = 0x84;
            return 2;
        } // Ä (already uppercase)
        if ((unsigned char)src[1] == 0x96)
        {
            dst[0] = 0xc3;
            dst[1] = 0x96;
            return 2;
        } // Ö (already uppercase)
    }
    // ASCII
    dst[0] = toupper((unsigned char)src[0]);
    return 1;
}

// Convert UTF-8 Å, Ä, Ö to lowercase
int UTF8_SwedishToLower(const char *src, char *dst)
{
    if ((unsigned char)src[0] == 0xc3)
    {
        if ((unsigned char)src[1] == 0x85)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa5;
            return 2;
        } // Å -> å
        if ((unsigned char)src[1] == 0x84)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa4;
            return 2;
        } // Ä -> ä
        if ((unsigned char)src[1] == 0x96)
        {
            dst[0] = 0xc3;
            dst[1] = 0xb6;
            return 2;
        } // Ö -> ö
        if ((unsigned char)src[1] == 0xa5)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa5;
            return 2;
        } // å (already lowercase)
        if ((unsigned char)src[1] == 0xa4)
        {
            dst[0] = 0xc3;
            dst[1] = 0xa4;
            return 2;
        } // ä (already lowercase)
        if ((unsigned char)src[1] == 0xb6)
        {
            dst[0] = 0xc3;
            dst[1] = 0xb6;
            return 2;
        } // ö (already lowercase)
    }
    dst[0] = tolower((unsigned char)src[0]);
    return 1;
}

int Input_YesNo(char choice)
{
    while (1)
    {
        if (choice == 'Y' || choice == 'y')
        {
            return 0;
        }
        else if (choice == 'N' || choice == 'n')
        {
            return 1;
        }
        else
        {
            printf("Invalid input. Enter Y/N: \n");
            // Clear the input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            scanf(" %c", &choice);
        }
    }
}