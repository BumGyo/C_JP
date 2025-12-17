#include <stdio.h>
#include <errno.h>
#include <string.h>

int cat_file(const char *name) {
    FILE *fp = fopen(name, "rb");
    if (!fp) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", name, strerror(errno));
        return 1;
    }

    int ch;
    while ((ch = getc(fp)) != EOF) { 
        if (putchar(ch) == EOF) {  
            fprintf(stderr, "書き込みエラー : %s\n", strerror(errno));
            fclose(fp);
            return 2;
        }
    }

    if (ferror(fp)) { // 読み取り途中のエラー
        fprintf(stderr, "読み取りエラー : %s: %s\n", name, strerror(errno));
        fclose(fp);
        return 3;
    }

    if (fclose(fp) == EOF) { // クローズ時のエラー
        fprintf(stderr, "クローズエラー : %s: %s\n", name, strerror(errno));
        return 4;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        int rc = cat_file(argv[i]);
        if (rc != 0) return rc; // 1つでも失敗したらその時点で終了
    }
    return 0;
}

