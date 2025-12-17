#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 1000000
#define MAX_CHAR   1000000
#define MAX_WORD   1024

struct item { char *id; int info; };

struct item table[TABLE_SIZE];
int count = 0;

int  char_top = 0;
char char_heap[MAX_CHAR];

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
    count = 0;
    char_top = 0;
}

int search_table(char *id)
{
    int lo = 0, hi = count - 1;
    int mid, cmp;

    while (lo <= hi) {
        mid = (lo + hi) / 2;
        cmp = strcmp(id, table[mid].id);
        if (cmp < 0)
            hi = mid - 1;
        else if (cmp > 0)
            lo = mid + 1;
        else
            return mid;
    }
    return -1;
}

void insert_table(char *id, int info)
{
    int pos;
    int move_count;

    if (count >= TABLE_SIZE) {
        printf("表があふれました。\n");
        exit(1);
    }

    int lo = 0, hi = count - 1, mid, cmp;
    while (lo <= hi) {
        mid = (lo + hi) / 2;
        cmp = strcmp(id, table[mid].id);
        if (cmp < 0)
            hi = mid - 1;
        else
            lo = mid + 1;
    }
    pos = lo;

    move_count = count - pos;
    if (move_count > 0) {
        memmove(&table[pos + 1],
                &table[pos],
                move_count * sizeof(struct item));
    }

    table[pos].id   = save_string(id);
    table[pos].info = info;
    count++;
}

void print_table(void)
{
    int i;
    for (i = 0; i < count; i++) {
        printf("%4d  %s\n", table[i].info, table[i].id);
    }
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
            if (p == -1)
                insert_table(word, 1);
            else
                table[p].info++;
        }
    }
    print_table();
    return 0;
}

