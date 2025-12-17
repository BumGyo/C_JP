#include <stdio.h>

static long long calls = 0;

long long fib_fast(int n)
{
    // 反復で高速に計算（重複計算なし）
    if (n == 0) return 0;
    if (n == 1) return 1;
    long long a = 0, b = 1;
    for (int i = 2; i <= n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}

int main(void)
{
    int n = 0;
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0;
    printf("%lld\n", fib_fast(n));
    
    return 0;
}
