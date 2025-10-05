#include <stdio.h>

int atoi1(const char* str) {
    int n = 0, sign = 1;

    while (*str == ' ' || *str == '\t') str++;

    if (*str == '+') str++;
    else if (*str == '-') {
        sign = -1;
        str++;
    }

    while (*str >= '0' && *str <= '9') {
        n = n * 10 + (*str - '0');
        str++;
    }

    return sign * n;
}

int main(void) {

    printf("atoi1(\"123\") = %d\n", atoi1("123"));        // output : 123
    printf("atoi1(\"   -45\") = %d\n", atoi1("   -45"));  // output : -45
    printf("atoi1(\"+77abc\") = %d\n", atoi1("+77abc"));  // output : 77
    printf("atoi1(\"0\") = %d\n", atoi1("0"));            // output : 0
    printf("atoi1(\"abc\") = %d\n", atoi1("abc"));        // output : 0
    
    return 0;
}


