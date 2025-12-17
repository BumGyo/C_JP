#include <stdio.h>

long long partition(int n, int x)
{
    if (n == 0) return 1;

    if (n < 0 || x == 0) return 0;

    return partition(n, x - 1) + partition(n - x, x);
}

long long p(int n)
{
    return partition(n, n);
}

int main(void)
{
    int n = 0;
    
    printf("n の値を入力してください : ");
    if (scanf("%d", &n) != 1) return 0; 
    printf("p(%d) = %lld\n", n, p(n));
    
    return 0;
}
