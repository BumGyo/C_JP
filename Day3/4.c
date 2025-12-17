#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

typedef int bool;
#define true 1
#define false 0

typedef struct {
    bool use_lineno;   // -n 指定の有無
    long h_start;      // 開始行
    long t_end;        // 終了行
} Opts;

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s [-n] [-h<num>] [-t<num>] [file ...]\n"
        "  -n      各行の先頭に通し行番号を付与\n"
        "  -h<num> 出力開始行を指定 (num>=1)\n"
        "  -t<num> 出力終了行を指定 (num>=1, -h と併用可)\n", prog);
}

// 10進整数の妥当性検査つき変換
static bool parse_pos_long(const char *s, long *out) {
    if (!s || !*s) return false;
    char *end = NULL;
    long v = strtol(s, &end, 10);
    if (*end != '\0') return false;
    if (v < 1 || v == LONG_MAX || v == LONG_MIN) return false;
    *out = v;
    return true;
}

// 1ストリームを処理
static int process_stream(FILE *fp, const Opts *opt, long *p_line, int *p_done) {
    int c;
    int bol = 1;          // 行頭フラグ
    long line = *p_line;  // 現在の通し行番号

    while ((c = fgetc(fp)) != EOF) {
        if (opt->t_end > 0 && line > opt->t_end) { *p_done = 1; break; }

        if (bol) {
            // 行頭に来たタイミングで行番号の出力可否を判断
            int in_range = (line >= opt->h_start) && (opt->t_end <= 0 || line <= opt->t_end);
            if (in_range && opt->use_lineno) {
                if (printf("%ld ", line) < 0) return -1;
            }
            bol = 0;
        }

        // 範囲内なら1文字ずつ出力、範囲外なら読み捨て
        if ((line >= opt->h_start) && (opt->t_end <= 0 || line <= opt->t_end)) {
            if (fputc(c, stdout) == EOF) return -1;
        }

        if (c == '\n') { line++; bol = 1; }
    }

    if (ferror(fp)) return -1;
    *p_line = line;
    return 0;
}

static int is_option(const char *s) {
    return s && s[0] == '-' && s[1] != '\0';
}

int main(int argc, char *argv[]) {
    Opts opt = { .use_lineno = false, .h_start = 1, .t_end = -1 }; // 既定
    const char *files[argc];
    int file_count = 0;

    // 引数解析
    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        if (is_option(a)) {
            if (strcmp(a, "-n") == 0) { opt.use_lineno = true; continue; }
            if (strncmp(a, "-h", 2) == 0) {
                long v;
                if (!parse_pos_long(a + 2, &v)) {
                    fprintf(stderr, "mycat: invalid -h option: '%s'\n", a);
                    print_usage(argv[0]);
                    return 2;
                }
                opt.h_start = v;
                continue;
            }
            if (strncmp(a, "-t", 2) == 0) {
                long v;
                if (!parse_pos_long(a + 2, &v)) {
                    fprintf(stderr, "mycat: invalid -t option: '%s'\n", a);
                    print_usage(argv[0]);
                    return 2;
                }
                opt.t_end = v;
                continue;
            }
            // 未知のオプション
            fprintf(stderr, "mycat: unknown option: '%s'\n", a);
            print_usage(argv[0]);
            return 2;
        }
        files[file_count++] = a;
    }

    // 論理的整合性チェック: -h > -t
    if (opt.t_end > 0 && opt.h_start > opt.t_end) {
        fprintf(stderr, "mycat: invalid range: -h%ld > -t%ld\n", opt.h_start, opt.t_end);
        print_usage(argv[0]);
        return 2;
    }

    int exit_code = 0;
    long line = 1;
    int done = 0;

    if (file_count == 0) {
        if (process_stream(stdin, &opt, &line, &done) != 0) {
            fprintf(stderr, "mycat: error reading from stdin\n");
            return 1;
        }
        return 0;
    }

    for (int i = 0; i < file_count && !done; ++i) {
        const char *name = files[i];
        FILE *fp = fopen(name, "rb");
        if (!fp) {
            fprintf(stderr, "mycat: cannot open '%s': %s\n", name, strerror(errno));
            exit_code = 1;
            continue;
        }
        if (process_stream(fp, &opt, &line, &done) != 0) {
            fprintf(stderr, "mycat: error while reading '%s'\n", name);
            exit_code = 1;
        }
        fclose(fp);
    }
    return exit_code;
}

