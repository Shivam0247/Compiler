#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void E();
void E_prime();
void T();
void T_prime();
void F();
void F_prime();
void P();

const char *input;
int current_token;

void next_token()
{
    while (isspace(*input))
        input++;
    current_token = *input++;
}

void check(int check)
{
    if (current_token == check)
    {
        next_token();
    }
    else
    {
        printf("Syntax error: expected '%c', got '%c'\n", check, current_token);
        exit(1);
    }
}

void E()
{
    T();
    E_prime();
}

void E_prime()
{
    if (current_token == '+')
    {
        next_token();
        T();
        E_prime();
    }
    else if (current_token == '-')
    {
        next_token();
        T();
        E_prime();
    }
}

void T()
{
    F();
    T_prime();
}

void T_prime()
{
    if (current_token == '*')
    {
        next_token();
        F();
        T_prime();
    }
    else if (current_token == '/')
    {
        next_token();
        F();
        T_prime();
    }
}

void F()
{
    P();
    F_prime();
}

void F_prime()
{
    if (current_token == '^')
    {
        next_token();
        F();
    }
}

void P()
{
    if (current_token == '(')
    {
        next_token();
        E();
        check(')');
    }
    else if (isalnum(current_token))
    {
        next_token();
    }
    else
    {
        printf("Syntax error: unexpected character '%c'\n", current_token);
        exit(1);
    }
}

void parse(const char *expr)
{
    input = expr;
    next_token();
    E();

    if (current_token != '\0')
    {
        printf("Syntax error: unexpected character '%c'\n", current_token);
    }
    else
    {
        printf("Parsing successful for input: %s\n", expr);
    }
}

int main()
{
    const char *expressions[] = {
        "(a+b*c^d)",
        "(a)",
        "((a^b))",
        "(a++b)",
    };

    int num_expressions = sizeof(expressions) / sizeof(expressions[0]);

    for (int i = 0; i < num_expressions; i++)
    {
        printf("Parsing expression: %s\n", expressions[i]);
        parse(expressions[i]);
        printf("\n");
    }

    return 0;
}
