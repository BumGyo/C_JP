/* calculator for postfix expressions */
/* examples  3 4 + 5 *  ==>  35 */
/*           3 4 5 * +  ==>  23 */

#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 100
int stack[STACK_SIZE];
int sp;

typedef enum { NUMBER, PLUS, MINUS, MULT, DIV, MOD, POW, OTHER, END } ttyp; 
ttyp token;

int c, num;
int badch = 0; 

static int peek_next_is_number(void) {

    int save_c = c;
    int buf[128];
    int n = 0;

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

// ユーザ向けエラー出力
static void user_error(const char* msg) {
    printf("%s\n", msg);
    exit(1);
}

// 2項演算に必要なオペランドの数を確認
static void require_two(const char* opname) {
    if (sp < 1) {
        if (sp == 0 && peek_next_is_number()) {
            printf("演算子に対応するオペランドの順序が正しくありません\n（後置記法で入力してください: 例 1 2 %s ）\n", opname);
            exit(1);
        } else {
            printf("オペランドが不足しています（%s）\n", opname);
            exit(1);
        }
    }
}

void initialize_stack(void)
{
    sp = -1;
}

void push(int x)
{
    if (sp < STACK_SIZE - 1)
        stack[++sp] = x;
    else
        user_error("式が長すぎます（スタックが満杯です）");
}

int pop(void)
{
    if (sp >= 0)
        return stack[sp--];
    else
        user_error("オペランドが不足しています"); // スタック空
    return 0;
}

// 整数べき乗（指数は0以上）
static int ipow(int base, int exp)
{
    if (exp < 0) user_error("負の指数は扱えません");
    int result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
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
        default:
            badch = c;            // 不正な文字を保存
            c = getchar();
            return OTHER;
    }
}

int main(void)
{
    int x;
    c = getchar();
    token = get_token();
    initialize_stack();

    while (token != END) {
        switch (token) {
            case NUMBER:
                push(num);
                break;
            case PLUS:
                require_two("+");
                push(pop() + pop());
                break;
            case MINUS:
                require_two("-");
                x = pop();
                push(pop() - x);
                break;
            case MULT:
                require_two("*");
                push(pop() * pop());
                break;
            case DIV:
                require_two("/");
                x = pop();
                if (x == 0) user_error("0 で割ることはできません");
                push(pop() / x);
                break;
            case MOD:
                require_two("%%");
                x = pop();
                if (x == 0) user_error("0 で割ることはできません");
                push(pop() % x);
                break;
            case POW:
                require_two("^");
                x = pop();
                push(ipow(pop(), x));
                break;
            case OTHER:
                {
                    char buf[64];
                    snprintf(buf, sizeof(buf), "無効な文字です: '%c'", (char)badch);
                    user_error(buf);
                }
                break;
            default:
                user_error("未知のエラーが発生しました");
        }
        token = get_token();
    }

    if (sp == 0)
        printf("%10d\n", pop());
    else if (sp > 0)
        user_error("オペレータが不足しています（値が複数残っています）");
    else
        user_error("オペランド(値)が不足しています");

    return 0;
}


