/* calculator for postfix expressions */
/* examples  3 4 + 5 *  ==>  35 */
/*           3 4 5 * +  ==>  23 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define STACK_SIZE 100
int stack[STACK_SIZE];
int sp;

typedef enum { NUMBER, PLUS, MINUS, MULT, DIV, MOD, POW, OTHER, END } ttyp;
ttyp token;

int c, num;
int badch = 0;

static void user_error(const char* msg) {
    printf("%s\n", msg);
    exit(1);
}

static int peek_next_is_number(void) {
    int save_c = c;
    int buf[128], n = 0;
    int t = save_c;
    while (t == ' ' || t == '\n' || t == '\t') {
        t = getchar();
        if (t == EOF) break;
        buf[n++] = t;
    }
    int isnum = ('0' <= t && t <= '9');
    for (int i = n - 1; i >= 0; --i) ungetc(buf[i], stdin);
    c = save_c;
    return isnum;
}

static void require_two(const char* opname) {
    if (sp < 1) {
        if (sp == 0 && peek_next_is_number()) {
            char buf[128];
            snprintf(buf, sizeof(buf), "演算子に対応するオペランドの順序が正しくありません", opname);
            user_error(buf);
        } else {
            char buf[64];
            snprintf(buf, sizeof(buf), "オペランドが不足しています（%s）", opname);
            user_error(buf);
        }
    }
}

static int add_checked(int a, int b, int* out) {
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)) return 1;
    *out = a + b; return 0;
}
static int sub_checked(int a, int b, int* out) {
    if ((b > 0 && a < INT_MIN + b) || (b < 0 && a > INT_MAX + b)) return 1;
    *out = a - b; return 0;
}
static int mul_checked(int a, int b, int* out) {
    if (a == 0 || b == 0) { *out = 0; return 0; }
    if (a == -1 && b == INT_MIN) return 1;
    if (b == -1 && a == INT_MIN) return 1;
    if (a > 0) {
        if (b > 0) { if (a > INT_MAX / b) return 1; }
        else       { if (b < INT_MIN / a) return 1; }
    } else {
        if (b > 0) { if (a < INT_MIN / b) return 1; }
        else       { if (a < INT_MAX / b) return 1; }
    }
    *out = a * b; return 0;
}
static int div_checked(int a, int b, int* out) {
    if (b == 0) user_error("0 で割ることはできません");
    if (a == INT_MIN && b == -1) return 1;
    *out = a / b; return 0;
}
static int mod_checked(int a, int b, int* out) {
    if (b == 0) user_error("0 で割ることはできません");
    *out = a % b; return 0;
}

static int ipow_checked(int base, int exp, int* out) {
    if (exp < 0) user_error("負の指数は扱えません");
    int result = 1;
    int cur = base;
    while (exp) {
        if (exp & 1) {
            if (mul_checked(result, cur, &result)) return 1;
        }
        if (exp >>= 1) {
            if (mul_checked(cur, cur, &cur)) return 1;
        }
    }
    *out = result; return 0;
}

void initialize_stack(void) { sp = -1; }
void push(int x) {
    if (sp < STACK_SIZE - 1) stack[++sp] = x;
    else user_error("式が長すぎます（スタックが満杯です）");
}
int pop(void) {
    if (sp >= 0) return stack[sp--];
    user_error("オペランドが不足しています");
    return 0;
}

ttyp get_token(void)
{
    while (c == ' ' || c == '\n' || c == '\t') c = getchar();
    switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            num = c - '0';
            for (;;) {
                c = getchar();
                if ('0' <= c && c <= '9') num = 10 * num + c - '0';
                else break;
            }
            return NUMBER;
        case '+': c = getchar(); return PLUS;
        case '-': c = getchar(); return MINUS;
        case '*': c = getchar(); return MULT;
        case '/': c = getchar(); return DIV;
        case '%': c = getchar(); return MOD;
        case '^': c = getchar(); return POW;
        case EOF: return END;
        default:  badch = c; c = getchar(); return OTHER;
    }
}

int main(void)
{
    int x, y, r;
    c = getchar();
    token = get_token();
    initialize_stack();

    while (token != END) {
        switch (token) {
            case NUMBER: push(num); break;

            case PLUS:
                require_two("+");
                y = pop(); x = pop();
                if (add_checked(x, y, &r)) user_error("演算結果が範囲を超えました（加算オーバーフロー）");
                push(r); break;

            case MINUS:
                require_two("-");
                y = pop(); x = pop();
                if (sub_checked(x, y, &r)) user_error("演算結果が範囲を超えました（減算オーバーフロー）");
                push(r); break;

            case MULT:
                require_two("*");
                y = pop(); x = pop();
                if (mul_checked(x, y, &r)) user_error("演算結果が範囲を超えました（乗算オーバーフロー）");
                push(r); break;

            case DIV:
                require_two("/");
                y = pop(); x = pop();
                if (div_checked(x, y, &r)) user_error("演算結果が範囲を超えました（除算オーバーフロー）");
                push(r); break;

            case MOD:
                require_two("%%");
                y = pop(); x = pop();
                if (mod_checked(x, y, &r)) user_error("演算結果が範囲を超えました（剰余オーバーフロー）");
                push(r); break;

            case POW:
                require_two("^");
                y = pop(); x = pop();
                if (ipow_checked(x, y, &r)) user_error("演算結果が範囲を超えました（べき乗オーバーフロー）");
                push(r); break;

            case OTHER: {
                char buf[64];
                snprintf(buf, sizeof(buf), "無効な文字です: '%c'", (char)badch);
                user_error(buf);
            }
        }
        token = get_token();
    }

    if (sp == 0) printf("= %d\n", pop());
    else if (sp > 0) user_error("オペレータが不足しています（値が複数残っています）");
    else user_error("オペランドが不足しています（値が残っていません）");

    return 0;
}

