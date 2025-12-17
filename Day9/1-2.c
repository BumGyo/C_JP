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
}

void insert_table(char *id, int info)
{
    struct cell *p;

    p = new_cell();
    p->id = save_string(id);
    p->info = info;
    p->next = head;
    head = p;
}

void print_list(void)
{
    struct cell *p;

    p = head;
    while (p != NULL) {
        printf("%s %d\n", p->id, p->info);
        p = p->next;
    }
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
    struct cell *p;

    initialize_table();
    insert_table("Copernicus", 1473);
    insert_table("Galilei", 1564);
    insert_table("Newton", 1643);
    insert_table("Maxwell", 1831);
    insert_table("Einstein", 1879);
    insert_table("Heisenberg", 1901);
    
    // print_list();

    p = search_table("Newton");
    if (p == NULL)
        printf("not found.\n");
    else
        printf("%d\n", p->info);
    return (0);
}

