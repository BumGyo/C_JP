#include <stdio.h>
void hanoi(int n, char *from, char *to, char *tmp);

int main(void)
{
    hanoi(3, "a", "b", "c");
    return 0;
}

void hanoi(int n, char *from, char *to, char *tmp)
{
    if (n == 0) return;          

    hanoi(n - 1, from, tmp, to);      // 一旦、上の n-1 枚を補助柱 tmp へ移す（from→tmp）
    printf("move disk %d from %s to %s\n", n, from, to);  // 最大の 1 枚を目的柱へ移動
    hanoi(n - 1, tmp, to, from);      // 補助柱の n-1 枚を目的柱へ移す（tmp→to）
}

