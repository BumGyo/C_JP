#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAP 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s rows_per_page columns\n", argv[0]);
        return 1;
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "rows and columns must be positive\n");
        return 1;
    }

    char **lines = NULL;
    int cap = 0;
    int n = 0;
    int maxlen = 0;
    char buf[256];

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
        int len = (int)strlen(buf);

        if (len > 0 && buf[len - 1] == '\n') {
            buf[--len] = '\0';
        }

        if (len > maxlen) {
            maxlen = len;
        }

        if (n >= cap) {
            int newcap = (cap == 0) ? INITIAL_CAP : cap * 2;
            char **tmp = realloc(lines, newcap * sizeof(*lines));
            if (tmp == NULL) {
                fprintf(stderr, "memory allocation failed\n");
                return 1;
            }
            lines = tmp;
            cap = newcap;
        }

        char *p = malloc((size_t)len + 1);
        if (p == NULL) {
            fprintf(stderr, "memory allocation failed\n");
            return 1;
        }
        strcpy(p, buf);
        lines[n++] = p;
    }

    if (n == 0) {
        free(lines);
        return 0;
    }

    int col_width = maxlen + 2;
    int page_size = rows * cols;
    int index = 0;

    while (index < n) {
        int remaining = n - index;
        int this_page = (remaining < page_size) ? remaining : page_size;

        for (int r = 0; r < rows; r++) {
            int printed = 0;

            for (int c = 0; c < cols; c++) {
                int idx = index + c * rows + r;
                if (idx >= index + this_page) {
                    continue;
                }

                if (printed > 0) {
                    putchar(' ');
                }

                printf("%-*s", col_width - 1, lines[idx]);
                printed = 1;
            }

            if (printed) {
                putchar('\n');
            }
        }

        index += this_page;
        if (index < n) {
            // putchar('\n'); 入力された行と列が終わる瞬間\n
        }
    }

    for (int i = 0; i < n; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}

