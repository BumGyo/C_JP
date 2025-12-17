#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR   1000000
#define MAX_WORD   1024

struct cell {
    char *id;
    int info;
    struct cell *next;
};

struct cell *head;
struct cell *tail;
int count = 0;

int  char_top = 0;
char char_heap[MAX_CHAR];

long long int strcmp_count = 0;

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

void initialize_table(void)
{
    head = NULL;
    tail = NULL;
    count = 0;
    char_top = 0;
}

struct cell *new_cell(void)
{
    struct cell *p;

    p = (struct cell *)malloc(sizeof(struct cell));
    if (p == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }
    return p;
}

struct cell *search_table(char *id)
{
    struct cell *p = head;

    while (p != NULL) {
        strcmp_count++;
        if (strcmp(id, p->id) == 0)
            return p;
        p = p->next;
    }
    return NULL;
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
}

struct item {
    char *id;
    int info;
};

int compare_item(const void *a, const void *b)
{
    const struct item *pa = (const struct item *)a;
    const struct item *pb = (const struct item *)b;
    return strcmp(pa->id, pb->id);
}

void print_table(void)
{
    int i;
    struct cell *p;
    struct item *array;

    array = (struct item *)malloc(sizeof(struct item) * count);
    if (array == NULL) {
        printf("memory allocation failed\n");
        exit(1);
    }

    i = 0;
    p = head;
    while (p != NULL) {
        array[i].id = p->id;
        array[i].info = p->info;
        i++;
        p = p->next;
    }

    qsort(array, count, sizeof(struct item), compare_item);

    for (i = 0; i < count; i++) {
        printf("%4d  %s\n", array[i].info, array[i].id);
    }

    free(array);
}

int main(void)
{
    int c, i;
    char word[MAX_WORD + 1];
    struct cell *p;
    clock_t start, end;

    initialize_table();

    start = clock();

    c = getchar();
    while (c != EOF) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
            c = getchar();
        } else {
            i = 0;
            while ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
                if (c >= 'A' && c <= 'Z')
                    c += 'a' - 'A';
                if (i >= MAX_WORD) {
                    printf("too long word\n");
                    exit(1);
                }
                word[i] = c;
                i++;
                c = getchar();
            }
            word[i] = 0;

            p = search_table(word);
            if (p == NULL)
                insert_table(word, 1);
            else
                p->info++;
        }
    }

    end = clock();

    print_table();
    printf("time = %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    printf("strcmp = %lld\n", strcmp_count);

    return 0;
}

