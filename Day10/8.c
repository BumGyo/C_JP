#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 257
#define MAX_CHAR 10000

struct item { char *id; int info; };
struct item table[M];
int mark[M];

char char_heap[MAX_CHAR];
int char_top = 0;

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
    int x = 0;
    while (*v != 0)
        x = 256 * x + (unsigned char)(*v++);
    x %= M;
    if (x < 0)
        x = -x;
    return x;
}

void initialize_table(void)
{
    int i;
    for (i = 0; i < M; i++)
        mark[i] = 0;
    char_top = 0;
}

int search_table(char *id)
{
    int x = hash(id);
    int ep = x;

    while (mark[x] != 0) {
        if (mark[x] == 1 && strcmp(table[x].id, id) == 0)
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
    int first_deleted = -1;

    while (mark[x] != 0) {
        if (mark[x] == 1 && strcmp(table[x].id, id) == 0) {
            printf("%s はすでに登録されています。\n", id);
            return;
        }
        if (mark[x] == 2 && first_deleted == -1)
            first_deleted = x;
        x = (x + 1) % M;
        if (x == ep)
            break;
    }

    if (first_deleted != -1) {
        x = first_deleted;
    } else if (mark[x] == 1) {
        printf("ハッシュ表が一杯です。これ以上登録できません。\n");
        return;
    }

    table[x].id   = save_string(id);
    table[x].info = info;
    mark[x] = 1;
}

void delete_table(char *id)
{
    int pos = search_table(id);
    if (pos == -1) {
        printf("%s は登録されていません。\n", id);
        return;
    }
    mark[pos] = 2;
    printf("%s を削除しました。\n", id);
}

int main(void)
{
    char command[100], name[100];
    int year;
    int pos;

    initialize_table();

    while (1) {
        if (scanf("%s", command) == EOF)
            break;

        if (command[0] == 'I') {
            if (scanf("%s %d", name, &year) != 2) {
                printf("入力形式が正しくありません。\n");
                break;
            }
            insert_table(name, year);

        } else if (command[0] == 'S') {
            if (scanf("%s", name) != 1) {
                printf("入力形式が正しくありません。\n");
                break;
            }
            pos = search_table(name);
            if (pos == -1)
                printf("%s は登録されていません。\n", name);
            else
                printf("%d\n", table[pos].info);

        } else if (command[0] == 'D') {
            if (scanf("%s", name) != 1) {
                printf("入力形式が正しくありません。\n");
                break;
            }
            delete_table(name);

        } else {
            printf("不明なコマンドです: %s\n", command);
        }
    }

    return 0;
}

