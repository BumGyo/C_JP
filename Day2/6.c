#include <stdio.h>
#include <errno.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    const char *A = argv[1], *B = argv[2];

    FILE *fa = fopen(A, "rb");  // バイナリ読み取り
    if (!fa) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", A, strerror(errno));
        return 2;
    }
    FILE *fb = fopen(B, "rb");
    if (!fb) {
        fprintf(stderr, "読み取りエラー : %s: %s\n", B, strerror(errno));
        fclose(fa);
        return 3;
    }

    unsigned long long pos = 0;
    int ca, cb;
    while (1) {
        ca = getc(fa);
        cb = getc(fb);
        pos++;
        
        // printf("ca = %d, cb =%d\n", ca, cb);

        if (ca == EOF || cb == EOF) {
            if (ca == EOF && cb == EOF) {
                // 完全一致
                printf("(1) identical\n");
                fclose(fa); fclose(fb);
                return 0;
            } else if (ca == EOF) {
                // Aの方が先にEOF → Aが短い
                printf("(3) EOF on A at byte %llu (A shorter)\n", pos);
                fclose(fa); fclose(fb);
                return 10;
            } else {
                // Bの方が先にEOF → Bが短い
                printf("(4) EOF on B at byte %llu (B shorter)\n", pos);
                fclose(fa); fclose(fb);
                return 11;
            }
        }

        if (ca != cb) {
            printf("(2) differ: byte %llu, A=0x%02X, B=0x%02X\n", pos, (unsigned char)ca, (unsigned char)cb);
            fclose(fa); fclose(fb);
            return 12;
        }
    }
}

