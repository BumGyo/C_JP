/* prog2.c */
#include <stdio.h>

int main(void) {
    int c;
    int prev = '\n';

    while ((c = getchar()) != EOF) {
        if (c == '\n' && prev == '\n') {
            continue;
        }
        putchar(c);
        prev = c;
    }

    return 0;
}

