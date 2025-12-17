#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1; // 引数不足エラー
    }

    const char *src = argv[1]; // 入力ファイル名
    const char *dst = argv[2]; // 出力ファイル名

    FILE *in  = fopen(src, "rb"); // 入力ファイルをバイナリ読み取りで開く
    if (!in) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", src, strerror(errno)); // オープン失敗
        return 2;
    }

    FILE *out = fopen(dst, "wb"); // 出力ファイルをバイナリ書き込みで開く/作成
    if (!out) {
        fprintf(stderr, "書き込みエラー : %s: %s\n", dst, strerror(errno)); // オープン/作成失敗
        fclose(in);
        return 3;
    }

    int ch;
    // EOF に達するまで
    while ((ch = getc(in)) != EOF) {
        if (putc(ch, out) == EOF) { 
            fprintf(stderr, "書き込みエラー : %s: %s\n", dst, strerror(errno));
            fclose(in);
            fclose(out);
            return 4;
        }
    }

    // 読み取り中のエラー
    if (ferror(in)) {
        fprintf(stderr, "読み取りエラー: %s: %s\n", src, strerror(errno));
        fclose(in);
        fclose(out);
        return 5;
    }

    // クローズ時のエラー
    if (fclose(in) == EOF) {
        fprintf(stderr, "クローズエラー : %s: %s\n", src, strerror(errno));
        fclose(out);
        return 6;
    }
    if (fclose(out) == EOF) {
        fprintf(stderr, "クローズエラー : %s: %s\n", dst, strerror(errno));
        return 7;
    }

    return 0;
}

