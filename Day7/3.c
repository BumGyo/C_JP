#include <stdio.h>

int main(void) {
    int c;

    while ((c = getchar()) != EOF) {
        if ('A' <= c && c <= 'Z') {
            c = c - 'A' + 'a';
        }
        putchar(c);
    }

    return 0;
}

