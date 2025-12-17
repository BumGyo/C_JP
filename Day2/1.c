#include <stdio.h>

int main() {
    FILE *fp;
    int c;
    int count = 0;

    fp = fopen("sample.txt", "r");
    if (fp == NULL) {
        perror("fail to open file");
        return 1;
    }

    while ((c = getc(fp)) != EOF) {
        count++;
    }

    fclose(fp);

    printf("sample.txt: %d byte\n", count);

    return 0;
}

