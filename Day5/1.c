/* calculator for postfix expressions */
/* examples  3 4 + 5 *  ==>  35 */
/*           3 4 5 * +  ==>  23 */

#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 100
int stack[STACK_SIZE];
int sp;

void initialize_stack(void)
{
    sp = -1;
}

void push(int x)
{
    if (sp < STACK_SIZE - 1)
        stack[++sp] = x;
    else {
        printf("stack full error.\n");
        exit(1);
    }
}

int pop(void)
{
    if (sp >= 0)
        return stack[sp--];   // 値をポップして返す
    else {
        printf("stack empty error.\n");
        exit(1);
    }
}

typedef enum { NUMBER, PLUS, MINUS, MULT, DIV, OTHER, END } ttyp;
ttyp token;

int c, num;

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
        case EOF: return END;
        default:  c = getchar(); return OTHER;
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
            case NUMBER: push(num); break;
            case PLUS:   push(pop() + pop()); break;
            case MINUS:  x = pop(); push(pop() - x); break;
            case MULT:   push(pop() * pop()); break;
            case DIV:    x = pop(); push(pop() / x); break;
            default:     printf("illegal character.\n"); exit(1);
        }
        token = get_token();
    }
    if (sp == 0) printf("%10d\n", pop());
    else { printf("expression syntax error.\n"); exit(1); }
    return 0;
}

