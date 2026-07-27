#include "parser.h"

#include <stdlib.h>

// handle errors with "panic mode"
static void parser_error(Parser *p, const char *err);
static void parser_synchronize(Parser *p);

// shift tokens
static void parser_advance(Parser *p);
// strict expect
static Token parser_consume(Parser *p, TokenType type, const char *err);
// optional expect
static bool parser_match(Parser *p, TokenType type);

// parser_next helper functions
static Node *parse_import_decl(Parser *p);
static Node *parse_struct_decl(Parser *p);
static Node *parse_enum_decl(Parser *p);
static Node *parse_type_decl(Parser *p);
static Node *parse_var_or_func_decl(Parser *p);

static void parser_error(Parser *p, const char *err) {
    if (p->panic_mode) return;

    p->errored = true;
    p->panic_mode = true;
    log_to_console(ERROR, "%s:%zu:%zu: %s", p->src_path, p->curr.line, p->curr.col, err);
    log_to_file(ERROR, DEFAULT_LOG_PATH, "%s:%zu:%zu: %s", p->src_path, p->curr.line, p->curr.col, err);
}

static void parser_synchronize(Parser *p) {
    p->panic_mode = false;

    while (p->curr.type != TOKEN_EOF) {
        if (p->prev.type == TOKEN_SEMI) return;

        switch (p->curr.type) {
            case TOKEN_STRUCT:
            case TOKEN_STATIC:
            case TOKEN_IMPORT:
            case TOKEN_IF:
            case TOKEN_WHILE:
            case TOKEN_FOR:
            case TOKEN_RETURN:
            case TOKEN_DEFER:
            case TOKEN_COMPTIME:
                return;
            default:
                break;
        }

        parser_advance(p);
    }
}

static void parser_advance(Parser *p) {
    do {
        p->prev = p->curr;
        p->curr = lexer_next(p->lex);
    } while (p->curr.type == TOKEN_UNKNOWN);
}

static Token parser_consume(Parser *p, TokenType type, const char *err) {
    if (p->curr.type == type) {
        parser_advance(p);
        return p->prev;
    }

    parser_error(p, err);
    return p->curr;
}

static bool parser_match(Parser *p, TokenType type) {
    if (p->curr.type == type) {
        parser_advance(p);
        return true;
    }
    return false;
}

static Node *parse_import_decl(Parser *p) {
    Token ident = parser_consume(p, TOKEN_IDENT, "Expected import path");
    if (p->errored) return NULL;

    const char *start_ptr = ident.view.data;
    const char *end_ptr = start_ptr + ident.view.len;

    while (parser_match(p, TOKEN_DOT)) {
        Token next = parser_consume(p, TOKEN_IDENT, "Expected submodule name after '.'");
        if (p->errored) return NULL;

        end_ptr = next.view.data + next.view.len;
    }

    parser_consume(p, TOKEN_SEMI, "Expected ';' after import path");

    Node *n = node_new(p->a, NODE_IMPORT, ident.line, ident.col, p->src_path);
    n->as.import_stmt.path.data = (char *)start_ptr;
    n->as.import_stmt.path.len = (size_t)(end_ptr - start_ptr);
    return n;
}

static Node *parse_struct_decl(Parser *p) {
    Token struct_name = parser_consume(p, TOKEN_IDENT, "Expected struct name");
    if (p->errored) return NULL;

    Node *n = node_new(p->a, NODE_STRUCT_DECL, struct_name.line, struct_name.col, p->src_path);
    n->as.struct_decl.name = struct_name;

    parser_consume(p, TOKEN_CURLY_LEFT, "Expected '{' before struct declaration");

    Node *head = NULL;
    Node *tail = NULL;
    size_t count = 0;

    while (p->curr.type != TOKEN_CURLY_RIGHT && p->curr.type != TOKEN_EOF) {
        Node *member = parse_var_or_func_decl(p);
        if (p->panic_mode) {
            parser_synchronize(p);
            continue; // try next members
        }
        if (!member) continue;

        member->next = NULL;
        if (!head) {
            head = member;
            tail = member;
        } else {
            tail->next = member;
            tail = member;
        }
        count++;
    }

    parser_consume(p, TOKEN_CURLY_RIGHT, "Expected '}' after struct declaration");

    n->as.struct_decl.fields = head;
    n->as.struct_decl.field_count = count;

    return n;
}

static Node *parse_enum_decl(Parser *p) {
    Token enum_name = parser_consume(p, TOKEN_IDENT, "Expected enum identifier");
    if (p->errored) return NULL;

    Node *n = node_new(p->a, NODE_ENUM_DECL, p->curr.line, p->curr.col, p->src_path);
    n->as.enum_decl.name = enum_name;

    parser_consume(p, TOKEN_CURLY_LEFT, "Expected '{' before enum declaration");

    Node *head = NULL;
    Node *tail = NULL;
    size_t count = 0;

    while (p->curr.type != TOKEN_CURLY_RIGHT && p->curr.type != TOKEN_EOF) {
        Node *member = parse_var_or_func_decl(p);
        if (p->panic_mode) {
            parser_synchronize(p);
            continue; // try next members
        }
        if (!member) continue;

        member->next = NULL;
        if (!head) {
            head = member;
            tail = member;
        } else {
            tail->next = member;
            tail = member;
        }
        count++;
    }

    parser_consume(p, TOKEN_CURLY_RIGHT, "Expected '}' after enum declaration");

    n->as.enum_decl.variants = head;
    n->as.enum_decl.variant_count = count;

    return n;
}

static Node *parse_type_decl(Parser *p);
static Node *parse_var_or_func_decl(Parser *p);

Parser parser_new(Lexer *lex, Arena *a, const char *path) {
    Parser p = (Parser) {
        .lex = lex,
        .a = a,
        .src_path = path,
        .errored = false,
        .panic_mode = false,
    };

    p.curr.type = TOKEN_UNKNOWN;
    parser_advance(&p);

    return p;
}

Node *parser_next(Parser *p) {
    if (p->curr.type == TOKEN_EOF) return NULL;

    if (parser_match(p, TOKEN_IMPORT)) return parse_import_decl(p);
    if (parser_match(p, TOKEN_STRUCT)) return parse_struct_decl(p);
    if (parser_match(p, TOKEN_ENUM)) return parse_enum_decl(p);
    if (parser_match(p, TOKEN_TYPE)) return parse_type_decl(p);

    Node *decl = parse_var_or_func_decl(p);

    if (p->panic_mode) {
        parser_synchronize(p);
        return NULL;
    }

    return decl;
}
