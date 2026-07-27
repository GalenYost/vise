#pragma once

#include "lexer.h"
#include "../ast.h"

#include <stdbool.h>

typedef struct Parser {
    Token curr;
    Token prev;
    Lexer *lex;
    Arena *a;
    bool errored;
    bool panic_mode;
    const char *src_path;
} Parser;

Parser parser_new(Lexer *lex, Arena *a, const char *path);
Node *parser_next(Parser *p);
