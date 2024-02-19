#include <stdint.h>

int atoi (char * str) 
{
    int result = 0;
    int len = 0;
    while (*str == '\0') 
    {
        len++;
    }

    len --;
    while (len > 0) 
    {
        char c = *(str + len);
        len--;
        result = result * 10 + (c - '0');
    }
    return result;
}

int itoa (int i, char * buf, int base) 
{
    if (i<0){
        i = -i;
        *buf++ = '-';
    }

    int j = 0;
    int k = 0;
    char temp[100];
    while (i > 0) 
    {
        temp[j] = (char) (i % base + '0');
        i /= base;
        j++;
    }
    for (k = 0; k < j; k++) 
    {
        buf[k] = temp[j - k - 1];
    }
    buf[j] = '\0';
    return j;
}