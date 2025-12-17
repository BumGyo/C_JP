#include <stdio.h>

int fact(int n)
{
    if (n == 0) return 1;

    return n * fact(n - 1);
}

int main(void)
{
    int n = 9;

    printf(" fact(%d) = %d \n", n, fact(n));
    return 0;
}

