#include <stdio.h>
#include <string.h>

#define MAX_LEN 64

int main(void) {
    char prev[MAX_LEN];
    char cur[MAX_LEN];
    int count = 0;
    int first = 1;

    while (fgets(cur, sizeof(cur), stdin) != NULL) {
        if (first) {
            strcpy(prev, cur);
            count = 1;
            first = 0;
        } else if (strcmp(prev, cur) == 0) {
            count++;
        } else {
            printf("%d %s", count, prev);
            strcpy(prev, cur);
            count = 1;
        }
    }

    if (!first) {
        printf("%d %s", count, prev);
    }

    return 0;
}

