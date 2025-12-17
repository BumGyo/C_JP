#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    const char *path = argv[1];
    FILE *fp = fopen(path, "r");
    if (!fp) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", path, strerror(errno));
        return 2;
    }

    long long pair_index = 0;
    while (1) {
        int a, b;
        int r = fscanf(fp, "%d,%d", &a, &b);
        if (r == 2) {
            printf("%d\n%d\n", a, b);
            printf("%d + %d = %d\n", a, b, a + b);
            pair_index++;
            continue;
        }
        if (r == EOF) {
            break;
        }
        // 入力形式エラー
        fprintf(stderr, "形式エラー");
        fclose(fp);
        return 3;
    }

    if (ferror(fp)) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", path, strerror(errno));
        fclose(fp);
        return 4;
    }

    if (fclose(fp) == EOF) {
        fprintf(stderr, "クローズエラー : %s: %s\n", path, strerror(errno));
        return 5;
    }

    return 0;
}

