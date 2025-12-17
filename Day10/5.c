#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 50000
#define MAX_CHAR 10000000
#define MAX_WORD 1024
#define HASH_A 37

struct item { char *id; int info; };
struct item table[M];
int mark[M];

char char_heap[MAX_CHAR];
int char_top = 0;
long long n_strcmp = 0;

char *save_string(char *s)
{
    int result = char_top;

    while(1) {
        if (char_top >= MAX_CHAR) {
            printf("文字列バッファがあふれました。\n");
            exit(1);
        }
        char_heap[char_top] = *s;
        char_top++;
        if (*s == 0)
            break;
        s++;
    }
    return &char_heap[result];
}

int hash(char *v)
{
    long long x = 0;
    while (*v != 0) {
        x = HASH_A * x + (unsigned char)(*v++);
    }
    long long r = x % M;
    if (r < 0)
        r += M;
    return (int)r;
}

void initialize_table(void)
{
    int i;
    for (i = 0; i < M; i++)
        mark[i] = 0;
}

int my_strcmp(const char *a, const char *b)
{
    n_strcmp++;
    return strcmp(a, b);
}

int search_table(char *id)
{
    int x = hash(id);
    int ep = x;

    while (mark[x] == 1) {
        if (my_strcmp(table[x].id, id) == 0)
            return x;
        x = (x + 1) % M;
        if (x == ep)
            return -1;
    }
    return -1;
}

void insert_table(char *id, int info)
{
    int x = hash(id);
    int ep = x;

    while (mark[x] == 1) {
        x = (x + 1) % M;
        if (x == ep) {
            printf("ハッシュ表が一杯です。これ以上登録できません。\n");
            return;
        }
    }

    table[x].id   = save_string(id);
    table[x].info = info;
    mark[x] = 1;
}

int compare_item(const void *a, const void *b)
{
    const struct item *pa = (const struct item *)a;
    const struct item *pb = (const struct item *)b;
    return strcmp(pa->id, pb->id);   /* qsort 用。ここでは比較回数を数えない */
}

void print_table(void)
{
    struct item *used;
    int i, n;

    used = (struct item *)malloc(sizeof(struct item) * M);
    if (used == NULL) {
        printf("メモリ確保に失敗しました。\n");
        exit(1);
    }

    n = 0;
    for (i = 0; i < M; i++) {
        if (mark[i] == 1) {
            used[n++] = table[i];
        }
    }

    qsort(used, n, sizeof(struct item), compare_item);

    for (i = 0; i < n; i++) {
        printf("%4d  %s\n", used[i].info, used[i].id);
    }

    printf("strcmp の回数: %lld 回\n", n_strcmp);

    free(used);
}

int main(void)
{
    int c, i, p;
    char word[MAX_WORD + 1];

    initialize_table();
    c = getchar();
    while (c != EOF) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            c = getchar();
        } else {
            i = 0;
            while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (i >= MAX_WORD) {
                    printf("単語が長すぎます。\n");
                    exit(1);
                }
                if (c >= 'A' && c <= 'Z')
                    c = c - 'A' + 'a';
                word[i] = c;
                i++;
                c = getchar();
            }
            word[i] = 0;

            p = search_table(word);
            if (p == -1)
                insert_table(word, 1);
            else
                table[p].info++;
        }
    }

    print_table();
    return 0;
}

