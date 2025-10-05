#include <stdio.h>

int strlen1(const char *s) {
    int len = 0;
    while (*s != '\0') {
        len++;
        s++;
    }
    return len;
}


int main(void) {
    printf("length: %d\n", strlen1("hello"));      // output : 5
    printf("length: %d\n", strlen1("abcdef"));     // output : 6
    printf("length: %d\n", strlen1(""));           // output : 0
    printf("length: %d\n", strlen1("a"));          // output : 1
    
    return 0;
}

