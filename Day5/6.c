/* calculator for postfix expressions */
/* examples  3 4 + 5 *  ==>  35 */
/*           3 4 5 * +  ==>  23 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

enum { STACK_SIZE = 100 };

static int stack[STACK_SIZE];
static int sp = -1;

static void user_error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static void init_stack(void) { sp = -1; }

static void push(int v) {
    if (sp >= STACK_SIZE - 1) user_error("式が長すぎます（スタックが満杯です）");
    stack[++sp] = v;
}

static int pop(void) {
    if (sp < 0) user_error("オペランドが不足しています");
    return stack[sp--];
}

static int ipow(int base, int exp) {
    if (exp < 0) user_error("負の指数は扱えません");
    int result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static void eval_line(char* line) {
    init_stack();
    char* p = line;
    int had_input = 0;

    while (*p) {
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '%' || *p == '^') {
            char op = *p++;
            if (op == '+') {
                if (sp < 1) user_error("オペランドが不足しています（+）");
                push(pop() + pop());
            } else if (op == '-') {
                if (sp < 1) user_error("オペランドが不足しています（-）");
                int b = pop(), a = pop();
                push(a - b);
            } else if (op == '*') {
                if (sp < 1) user_error("オペランドが不足しています（*）");
                push(pop() * pop());
            } else if (op == '/') {
                if (sp < 1) user_error("オペランドが不足しています（/）");
                int b = pop(); if (b == 0) user_error("0 で割ることはできません");
                int a = pop();
                push(a / b);
            } else if (op == '%') {
                if (sp < 1) user_error("オペランドが不足しています（%）");
                int b = pop(); if (b == 0) user_error("0 で割ることはできません");
                int a = pop();
                push(a % b);
            } else {
                if (sp < 1) user_error("オペランドが不足しています（^）");
                int e = pop(), a = pop();
                push(ipow(a, e));
            }
            had_input = 1;
            continue;
        }

        char* endp;
        long v = strtol(p, &endp, 10);
        if (endp == p) {
            char buf[64];
            snprintf(buf, sizeof(buf), "無効なトークンです: '%c'", *p);
            user_error(buf);
        }
        if (v < INT_MIN || v > INT_MAX) user_error("整数の範囲を超えています");
        push((int)v);
        had_input = 1;
        p = endp;
    }

    if (!had_input) return;
    if (sp == 0) {
        printf("%10d\n", stack[sp--]);
    } else if (sp > 0) {
        user_error("オペレータが不足しています（値が複数残っています）");
    } else {
        user_error("オペランドが不足しています（値が残っていません）");
    }
}

int main(void) {
    char buf[1024];
    
    while (fgets(buf, sizeof(buf), stdin)) {
        eval_line(buf);
    }
    return 0;
}

