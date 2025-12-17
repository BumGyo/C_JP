#include <stdio.h>

long long fib(int n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    // 再帰: f(n) = f(n-1) + f(n-2)
    return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    int n;
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0;

    printf("%lld\n", fib(n));
    return 0;
}
