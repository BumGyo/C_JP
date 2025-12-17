#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 10000000

int char_top = 0;
char char_heap[MAX_CHAR];

struct cell {
    char *id;
    int info;
    struct cell *next;
};
struct cell *head;
struct cell *tail;

int count;

char *save_string(char *s)
{
    int result = char_top;

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

struct cell *new_cell(void)
{
    struct cell *p;

    p = (struct cell *)malloc(sizeof(struct cell));
    if (p == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }
    return (p);
}

void initialize_table(void)
{
    head = NULL;
    tail = NULL;
    char_top = 0;
    count = 0;
}

void print_list(void)
{
    struct cell *p;
    int i = 0;

    printf("count = %d\n", count);
    p = head;
    while (p != NULL) {
        printf("%d: %s %d\n", i, p->id, p->info);
        p = p->next;
        i++;
    }
    printf("----\n");
}

void insert_table(char *id, int info)
{
    struct cell *p;

    p = new_cell();
    p->id = save_string(id);
    p->info = info;
    p->next = NULL;

    if (head == NULL) {
        head = p;
        tail = p;
    } else {
        tail->next = p;
        tail = p;
    }

    count++;

    print_list();
}

struct cell *search_table(char *id)
{
    struct cell *p;

    p = head;
    while (p != NULL) {
        if (strcmp(id, p->id) == 0)
            return (p);
        p = p->next;
    }
    return (NULL);
}

int main(void)
{
    char command[100], name[100];
    int year;
    struct cell *p;

    initialize_table();

    while (1) {
        if (scanf("%s %s", command, name) == EOF)
            break;

        if (command[0] == 'I') {
            if (scanf("%d", &year) != 1) {
                printf("年の入力エラーです。\n");
                break;
            }
            p = search_table(name);
            if (p != NULL) {
                printf("%s はすでに登録されています。\n", name);
            } else {
                insert_table(name, year);
            }
        } else if (command[0] == 'S') {
            p = search_table(name);
            if (p == NULL) {
                printf("%s は登録されていません。\n", name);
            } else {
                printf("%d\n", p->info);
            }
        } else {
            printf("不明なコマンドです: %s\n", command);
        }
    }

    return (0);
}

