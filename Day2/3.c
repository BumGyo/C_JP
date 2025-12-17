#include <stdio.h>
#include <errno.h>
#include <string.h>

int main(void) {
    const char *in_name  = "sample1.txt";
    const char *out1_name = "sample2.txt";
    const char *out2_name = "sample3.txt";

    FILE *in  = fopen(in_name, "rb");   // バイナリ読み取りで開く
    if (!in) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", in_name, strerror(errno));
        return 1;
    }

    FILE *out1 = fopen(out1_name, "wb"); // バイナリ書き込みで開く/作成
    if (!out1) {
        fprintf(stderr, "書き込みエラー : %s: %s\n", out1_name, strerror(errno));
        fclose(in);
        return 2;
    }

    FILE *out2 = fopen(out2_name, "wb"); // バイナリ書き込みで開く/作成
    if (!out2) {
        fprintf(stderr, "書き込みエラー : %s: %s\n", out2_name, strerror(errno));
        fclose(in);
        fclose(out1);
        return 3;
    }

    int ch;
    // EOFまで1バイトずつ読み取り、2つの出力へ同一データを書き込む
    while ((ch = getc(in)) != EOF) {
        if (putc(ch, out1) == EOF || putc(ch, out2) == EOF) {
            fprintf(stderr, "書き込みエラー : %s or %s\n", out1_name, out2_name);
            fclose(in);
            fclose(out1);
            fclose(out2);
            return 4;
        }
    }

    // 読み取りエラー
    if (ferror(in)) {
        fprintf(stderr, "読み取りエラー: %s\n", in_name);
        fclose(in);
        fclose(out1);
        fclose(out2);
        return 5;
    }

    // クローズ時のエラー
    if (fclose(in) == EOF) {
        fprintf(stderr, "クローズエラー : %s\n", in_name);
        fclose(out1);
        fclose(out2);
        return 6;
    }
    if (fclose(out1) == EOF) {
        fprintf(stderr, "クローズエラー : %s\n", out1_name);
        fclose(out2);
        return 7;
    }
    if (fclose(out2) == EOF) {
        fprintf(stderr, "クローズエラー : %s\n", out2_name);
        return 8;
    }

    return 0;
}

