#include <stdio.h>

char* strcpy1(char* dest, const char* src) {
    char* ptr = dest;
    while (*src) {
        *ptr = *src;
        ptr++;
        src++;
    }
    *ptr = '\0';  // last index null
    return dest;
}

int main(void) {
    char dest[20];

    strcpy1(dest, "hello");
    printf("copy result: %s\n", dest);  // output : hello

    strcpy1(dest, "abcde");
    printf("copy result: %s\n", dest);  // output : abcde

    strcpy1(dest, "");
    printf("copy result: %s\n", dest);  // output : 

    return 0;
}
