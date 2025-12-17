#include <stdio.h>

void print_base(unsigned int x, int b)
{
    if (x < (unsigned)b) {
        putchar('0' + (int)x);
        return;
    }
    print_base(x / (unsigned)b, b);
    putchar('0' + (int)(x % (unsigned)b));
}

int main(void)
{
    unsigned int x = 17;
    printf("x の値を入力してください : ");
    scanf("%d", &x);

    int b = 3;
    printf("b の値を入力してください : ");
    scanf("%d", &b);


    print_base(x, b);
    putchar('\n');
    return 0;
}

