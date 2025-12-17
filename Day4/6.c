#include <stdio.h>

static int n;
static int a[16];

void generate(int level)
{
    if (level >= n) {
        for (int i = 0; i < n; i++) {
            printf("%d ", a[i]);
        }
        putchar('\n');
        return;
    }

    for (int v = 1; v <= n; v++) {
        a[level] = v;          // level 番目の値を記録
        generate(level + 1);   // 次の桁を生成
    }
}

int main(void)
{
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 8) return 0;
    generate(0);
    return 0;
}

