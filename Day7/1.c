#include <stdio.h>

int main(void) {
    int c;

    while ((c = getchar()) != EOF) {
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) {
            putchar(c);
        } else {
            putchar('\n');
        }
    }

    return 0;
}

