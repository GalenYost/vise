#pragma once

#include "../ast.h"
#include "../utils.h"

typedef struct {
    const char *start;
    const char *current;
    size_t line;
    size_t col;
    Arena *arena;
} Lexer;

Lexer lexer_new(StringStream ss, Arena *a);
Token lexer_next(Lexer *lex);
