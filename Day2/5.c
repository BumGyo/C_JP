#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

static int is_printable(int c) {
    return (32 <= c && c <= 126);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const char *name = argv[1];
    FILE *fp = fopen(name, "rb");
    if (!fp) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", name, strerror(errno));
        return 2;
    }

    unsigned long long counts[256] = {0};

    int ch;
    while ((ch = getc(fp)) != EOF) {
        counts[(unsigned char)ch]++;
    }

    if (ferror(fp)) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", name, strerror(errno));
        fclose(fp);
        return 3;
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "クローズエラー : %s: %s\n", name, strerror(errno));
        return 4;
    }

    printf("code  char  count\n");
    printf("------------------------\n");
    for (int i = 0; i < 256; i++) {
        if (counts[i] == 0) continue;
        if (is_printable(i)) {
            printf("%3d   %c     %llu\n", i, i, counts[i]);
        } else {
            printf("%3d         %llu\n", i, counts[i]);
        }
    }

    return 0;
}

