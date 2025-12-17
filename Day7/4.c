#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000000
#define MAX_LEN     64

static int cmp_line(const void *a, const void *b) {
    const char *const *pa = (const char *const *)a;
    const char *const *pb = (const char *const *)b;
    return strcmp(*pa, *pb);
}

int main(void) {
    static char buf[MAX_LINE][MAX_LEN];
    char *line[MAX_LINE];
    int n = 0;

    while (n < MAX_LINE && fgets(buf[n], sizeof(buf[n]), stdin) != NULL) {
        line[n] = buf[n];
        n++;
    }

    qsort(line, n, sizeof(char *), cmp_line);

    for (int i = 0; i < n; i++) {
        fputs(line[i], stdout);
    }

    return 0;
}

