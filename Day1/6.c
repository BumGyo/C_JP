#include <stdio.h>


char* strstr1(const char* s1, const char* s2) {
    if (*s2 == '\0') return (char*)s1;  

    for (; *s1 != '\0'; s1++) {
        const char *p1 = s1;
        const char *p2 = s2;
        while (*p1 && *p2 && (*p1 == *p2)) {
            p1++;
            p2++;
        }
        if (*p2 == '\0') {
            return (char*)s1;
        }
    }
    return NULL;
}

void test_strstr1(const char *s1, const char *s2) {
    char *p = strstr1(s1, s2);
    if (p == NULL)
        printf("[%s], [%s] -> [NULL]\n", s1, s2);
    else
        printf("[%s], [%s] -> [%s]\n", s1, s2, p);
}

int main(void) {
    
    test_strstr1("abcddddef", "dde");
    test_strstr1("abcabc", "abc");
    test_strstr1("print", "int");
    test_strstr1("abc", "");
    test_strstr1("", "");
    test_strstr1("age:20, name:tom", "name:");
    test_strstr1("This is a pen", "never");
    
    return 0;
}

