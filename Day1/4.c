#include <stdio.h>

char* strcat1(char *dest, const char *src) {
    char *ptr = dest;


    while (*ptr != '\0') {
        ptr++;
    }

    while (*src != '\0') {
        *ptr = *src;
        ptr++;
        src++;
    }

    // last index == NULL
    *ptr = '\0';

    return dest;
}


int main() {
    char str[20] = "abc";
    printf("before = %s\n", str);  // output : abc

    strcat1(str, "def");
    printf("result = %s\n", str);  // output: abcdef

    return 0;
}

