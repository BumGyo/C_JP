#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(void) {
    const char *name = "integers.txt";
    FILE *fp = fopen(name, "r");
    if (!fp) {
        fprintf(stderr, "読み取りエラー :  %s: %s\n", name, strerror(errno));
        return 1;
    }

    int x;
    long long sum = 0;
    long long count = 0;

    // EOF に達するまで
    while (fscanf(fp, "%d", &x) == 1) {
        printf("%d\n", x);
        sum = sum + x;
        count++;
    }

    if (ferror(fp)) {
        fprintf(stderr, "読み取りエラー :  %s: %s\n", name, strerror(errno));
        fclose(fp);
        return 2;
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "クローズエラー :  %s: %s\n", name, strerror(errno));
        return 3;
    }
    
    printf("sum=%lld\n", sum);
    printf("count=%lld\n", count);
    return 0;
}

