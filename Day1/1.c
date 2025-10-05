#include <stdio.h>

// pointer
char* strchr1(const char* s, int c) {
    while (*s) {
        if (*s == c) return (char*)s;
        s++;
    }
    return NULL;
}

// index
char* strchr2(const char* s, int c) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == c) return (char*)(s + i);
        i++;
    }
    return NULL;
}

// test
void test_strchr(const char* str, int ch) {
    char* pos1 = strchr1(str, ch);
    char* pos2 = strchr2(str, ch);

    if(pos1)
        printf("strchr1: [%s], [%c] -> [%s]\n", str, ch, pos1);
    else
        printf("strchr1: [%s], [%c] -> [NULL]\n", str, ch);

    if(pos2)
        printf("strchr2: [%s], [%c] -> [%s]\n", str, ch, pos2);
    else
        printf("strchr2: [%s], [%c] -> [NULL]\n", str, ch);
}

int main(void) {
    test_strchr("abcdef", 'd');
    test_strchr("aaaaaa", 'a');
    test_strchr("abcdef", 'z');
    test_strchr("", 'a');
    test_strchr("12345", '5');
    
    return 0;
}
