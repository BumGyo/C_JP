#include <stdio.h>

char* strpbrk1(const char* s, const char* accept) {
    for (; *s != '\0'; s++) {
        for (const char *a = accept; *a != '\0'; a++) {
            if (*s == *a) return (char*)s;
        }
    }
    return NULL;
}

int main(void) {

    char *p;
    p = strpbrk1("abcdef", "xyza");   // output : abcdef
    printf("%s\n", p ? p : "NULL");

    p = strpbrk1("hello world", "oW"); // output : o world
    printf("%s\n", p ? p : "NULL");

    p = strpbrk1("test", "xyz");    // output : NULL
    printf("%s\n", p ? p : "NULL");

    return 0;
}

