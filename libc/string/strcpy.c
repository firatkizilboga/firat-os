#include <string.h>
char * strcpy(char* destination, const char* source){
    *destination = *source;
    if (*source == '\0')
    {
        return destination;
    }
    strcpy(destination + 1, source + 1);
    return destination;
}