#include <stdio.h>
#include <stdbool.h>

static int n;
static int perm[16]; 
static bool used[16];
static int count = 0;

// 位置の差 == 値の差になっていないか衝突チェック
static bool ok_place(int level, int v)
{
    // level に v を置いても良いかを確認
    for (int k = 0; k < level; k++) {
        int dl = level - k;          // 位置の差
        int dv = v - perm[k];        // 値の差
        if (dl < 0) dl = -dl;
        if (dv < 0) dv = -dv;
        if (dl == dv) return false;
    }
    return true;
}

void generate(int level)
{
    if (level >= n) {
        for (int i = 0; i < n; i++) {
            if (i) putchar(' ');
            printf("%d", perm[i]);
        }
        putchar('\n');
        count++;
        return;
    }
    
    for (int v = 1; v <= n; v++) {
        if (used[v]) continue;
        if (!ok_place(level, v)) continue;
        used[v] = true;
        perm[level] = v;
        generate(level + 1);
        used[v] = false;
    }
}

int main(void)
{
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0;
    if (n < 1 || n > 9) return 0;
    generate(0);
    printf("nの解の個数 : %d\n", count);
    return 0;
}

