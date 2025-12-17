#include <stdio.h>
#include <stdbool.h>

static int n;
static int perm[10];     // n <= 9
static bool used[10];    // 1..n を使用済み管理

void generate(int level)
{
    if (level >= n) {
        for (int i = 0; i < n; i++) {
            if (i) putchar(' ');
            printf("%d", perm[i]);
        }
        putchar('\n');
        return;
    }
    
    for (int v = 1; v <= n; v++) {
        if (used[v]) continue;   // すでに使った数はスキップ
        used[v] = true;          // 使用開始
        perm[level] = v;         // level 番目に配置
        generate(level + 1);     // 次の位置へ
        used[v] = false;         // 戻って未使用に戻す
    }
}

int main(void)
{
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 9) return 0; 
    generate(0);
    return 0;
}

