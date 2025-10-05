#include <stdio.h>

int strcmp1(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return (unsigned char)*str1 - (unsigned char)*str2;
}


int main(void) {
    printf("strcmp1(\"abc\", \"abc\") = %d\n", strcmp1("abc", "abc"));   // 0 (Same)
    printf("strcmp1(\"abc\", \"abcd\") = %d\n", strcmp1("abc", "abcd")); // - (str1 < str2)
    printf("strcmp1(\"bac\", \"abc\") = %d\n", strcmp1("bac", "abc"));   // + (str1 > str2)
    printf("strcmp1(\"\", \"\") = %d\n", strcmp1("", ""));               // 0 (Same)
    
    return 0;
}

