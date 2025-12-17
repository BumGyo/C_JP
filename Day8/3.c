#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000000
#define MAX_CHAR   1000000

struct item { char *id; int info; };

struct item table[TABLE_SIZE];
int count;

int  char_top = 0;
char char_heap[MAX_CHAR];

char *save_string(char *s)
{
    int result;

    result = char_top;
    for (;;) {
        if (char_top >= MAX_CHAR) {
            printf("string buffer overflow\n");
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

void initialize_table(void)
{
    count = 0;
    char_top = 0;
}

void print_table(void)
{
    int i;
    printf("count = %d\n", count);
    for (i = 0; i < count; i++) {
        printf("%d: %s %d\n", i, table[i].id, table[i].info);
    }
    printf("----\n");
}

void insert_table(char *id, int info)
{
    if (count >= TABLE_SIZE) {
        printf("表があふれました。\n");
        exit(1);
    }

    table[count].id   = save_string(id);
    table[count].info = info;
    count++;

    print_table();
}

int search_table(char *id)
{
    int i;

    for (i = 0; i < count; i++)
        if (strcmp(id, table[i].id) == 0)
            return i;
    return -1;
}

int main(void)
{
    char command[100], name[100];
    int year;
    int pos;

    initialize_table();

    while (1) {
        if (scanf("%s %s", command, name) == EOF)
            break;

        if (command[0] == 'I') {
            if (scanf("%d", &year) != 1) {
                printf("年の入力エラーです。\n");
                break;
            }
            pos = search_table(name);
            if (pos != -1) {
                printf("%s はすでに登録されています。\n", name);
            } else {
                insert_table(name, year);
            }
        } else if (command[0] == 'S') {
            pos = search_table(name);
            if (pos == -1) {
                printf("%s は登録されていません。\n", name);
            } else {
                printf("%d\n", table[pos].info);
            }
        } else {
            printf("不明なコマンドです: %s\n", command);
        }
    }

    return (0);
}
