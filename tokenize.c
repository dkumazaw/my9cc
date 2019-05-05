#include "9cc.h"

// Returns true if a char is [a-zA-Z_]
int is_al(char c)
{
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           (c == '_');
}

// Returns true if a char is [a-zA-Z0-9_]
int is_alnum(char c)
{
    return ('a' <= c && c <= 'z') ||
           ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           (c == '_');
}

/*
tokenize 
*/
void tokenize(char *p, Vector *tokens)
{
    while (*p)
    {
        // Skip whitespace
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6]))
        {
            Token *token = new_token();
            token->ty = TK_RETURN;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 6;
            continue;
        }

        if (is_al(*p))
        {
            Token *token = new_token();
            token->ty = TK_IDENT;
            token->input = p;

            p++;
            while (is_alnum(*p))
                p++;

            token->name = strndup(token->input, (size_t)(p - token->input + 1));
            vec_push(tokens, (void *)token);
            continue;
        }

        if (strncmp(p, "==", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_EQ;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, "!=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_NE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, "<=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_LE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (strncmp(p, ">=", 2) == 0)
        {
            Token *token = new_token();
            token->ty = TK_GE;
            token->input = p;
            vec_push(tokens, (void *)token);
            p += 2;
            continue;
        }

        if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == '<' || *p == '>' || *p == ';' || *p == '=')
        {
            Token *token = new_token();
            token->ty = *p;
            token->input = p;
            vec_push(tokens, (void *)token);
            p++;
            continue;
        }

        if (isdigit(*p))
        {
            Token *token = new_token();
            token->ty = TK_NUM;
            token->input = p;
            token->val = strtol(p, &p, 10);
            vec_push(tokens, (void *)token);
            continue;
        }

        fprintf(stderr, "Cannot tokenize: %s\n", p);
        exit(1);
    }

    Token *token = new_token();
    token->ty = TK_EOF;
    token->input = p;
    vec_push(tokens, (void *)token);
}