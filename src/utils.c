#include "utils.h"
#include "arena.h"
#include "ast.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void print_node_recursive(Node *n);

StringStream sstream_new(char *str) {
    return (StringStream) {
        .data = str,
        .len = strlen(str),
    };
}

StringStream sstream_dump(Arena *a, StringStream ss) {
    char *dest = (char *)arena_alloc(a, ss.len);
    memcpy(dest, ss.data, ss.len);
    
    return (StringStream){
        .data = dest,
        .len = ss.len,
    };
}

char *sstream_to_cstr(Arena *a, StringStream ss) {
    char *cstr = (char *)arena_alloc(a, ss.len + 1);
    memcpy(cstr, ss.data, ss.len);
    cstr[ss.len] = '\0';

    return cstr;
}

StringStream read_file(const char *path) {
    StringStream ss = {0};

    FILE *f = fopen(path, "rb");
    if (!f) {
        LOG(ERROR, "Could not open file for reading: %s", path);
        return ss;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize < 0) {
        LOG(ERROR, "Could not read file size: %s", path);
        fclose(f);
        return ss;
    }

    char *buf = (char*)malloc(fsize + 1);

    if (!buf) {
        LOG(FATAL, "Could not allocate %ld bytes to read file %s", fsize, path);
        fclose(f);
        exit(1);
    }

    size_t bytes_read = fread(buf, 1, fsize, f);

    if (bytes_read < (size_t)fsize) {
        LOG(ERROR, "Failed to read entire file: %s", path);
        free(buf);
        fclose(f);
        return ss;
    }

    buf[bytes_read] = '\0';
    fclose(f);

    ss.data = buf;
    ss.len = strlen(buf);

    return ss;
}

#define CONSOLE_PREFIX_FMT "[%s%s%s] "
#define FILE_PREFIX_FMT "[%s] "

#define CYAN "\033[36m"
#define RED "\033[91m"
#define GREEN "\033[92m"

#define DARK_RED "\033[31m"
#define DARK_YELLOW "\033[33m"

#define RESET_COLOR "\033[0m"

static const char *LOG_PREFIXES[] = {
    "DEBUG", "INFO",
    "WARN", "ERROR",
    "FATAL",
};

static const char *LOG_PREFIX_COLORS[] = {
    CYAN,
    GREEN,
    DARK_YELLOW,
    RED, DARK_RED,
};

void log_to_console(LogLevel level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    printf(CONSOLE_PREFIX_FMT, LOG_PREFIX_COLORS[level], LOG_PREFIXES[level], RESET_COLOR);
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
}

void log_to_file(LogLevel level, const char *path, const char *fmt, ...) {
    FILE *f = fopen(path, "a");
    if (!f) return;

    va_list args;
    va_start(args, fmt);

    fprintf(f, FILE_PREFIX_FMT, LOG_PREFIXES[level]);
    vfprintf(f, fmt, args);
    fprintf(f, "\n");

    va_end(args);
    fclose(f);
}

void print_token(Token t) {
    static const char *words[] = {
        // SYMBOLS
        [TOKEN_PLUS] = "TOKEN_PLUS",
        [TOKEN_MINUS] = "TOKEN_MINUS",
        [TOKEN_STAR] = "TOKEN_STAR",
        [TOKEN_SLASH] = "TOKEN_SLASH",
        [TOKEN_EQ] = "TOKEN_EQ",
        [TOKEN_PAREN_LEFT] = "TOKEN_PAREN_LEFT",
        [TOKEN_PAREN_RIGHT] = "TOKEN_PARENT_RIGHT",
        [TOKEN_BRACE_LEFT] = "TOKEN_BRACE_LEFT",
        [TOKEN_BRACE_RIGHT] = "TOKEN_BRACE_RIGHT",
        [TOKEN_CURLY_LEFT] = "TOKEN_CURLY_LEFT",
        [TOKEN_CURLY_RIGHT] = "TOKEN_CURLY_RIGHT",
        [TOKEN_BANG] = "TOKEN_BANG",
        [TOKEN_AT] = "TOKEN_AT",
        [TOKEN_HASH] = "TOKEN_HASH",
        [TOKEN_DOLLAR] = "TOKEN_DOLLAR",
        [TOKEN_PERCENT] = "TOKEN_PERCENT",
        [TOKEN_QUOTE] = "TOKEN_QUOTE",
        [TOKEN_DOUBLE_QUOTE] = "TOKEN_DOUBLE_QUOTE",
        [TOKEN_QUESTION] = "TOKEN_QUESTION",
        [TOKEN_COMMA] = "TOKEN_COMMA",
        [TOKEN_SEMI] = "TOKEN_SEMI",
        [TOKEN_COLON] = "TOKEN_COLON",
        [TOKEN_DOT] = "TOKEN_DOT",
        // OPERATORS
        [TOKEN_PLUS_PLUS] = "TOKEN_PLUS_PLUS",
        [TOKEN_PLUS_EQ] = "TOKEN_PLUS_EQ",
        [TOKEN_MINUS_MINUS] = "TOKEN_MINUS_MINUS",
        [TOKEN_MINUS_EQ] = "TOKEN_MINUS_EQ",
        [TOKEN_MULTI_EQ] = "TOKEN_MULTI_EQ",
        [TOKEN_DIV_EQ] = "TOKEN_DIV_EQ",
        [TOKEN_EQ_EQ] = "TOKEN_EQ_EQ",
        [TOKEN_NOT_EQ] = "TOKEN_NOT_EQ",
        [TOKEN_LT] = "TOKEN_LT",
        [TOKEN_LT_EQ] = "TOKEN_LT_EQ",
        [TOKEN_GT] = "TOKEN_GT",
        [TOKEN_GT_EQ] = "TOKEN_GT_EQ",
        [TOKEN_POW] = "TOKEN_POW",
        [TOKEN_XOR] = "TOKEN_XOR",
        [TOKEN_LOGICAL_AND] = "TOKEN_LOGICAL_AND",
        [TOKEN_LOGICAL_OR] = "TOKEN_LOGICAL_OR",
        [TOKEN_BIT_OR] = "TOKEN_BIT_OR",
        [TOKEN_BIT_OR_EQ] = "TOKEN_BIT_OR_EQ",
        [TOKEN_BIT_AND] = "TOKEN_BIT_AND",
        [TOKEN_BIT_AND_EQ] = "TOKEN_BIT_AND_EQ",
        [TOKEN_SHIFT_LEFT] = "TOKEN_SHIFT_LEFT",
        [TOKEN_SHIFT_LEFT_EQ] = "TOKEN_SHIFT_LEFT_EQ",
        [TOKEN_SHIFT_RIGHT] = "TOKEN_SHIFT_RIGHT",
        [TOKEN_SHIFT_RIGHT_EQ] = "TOKEN_SHIFT_RIGHT_EQ",
        [TOKEN_BIT_NOT] = "TOKEN_BIT_NOT",
        [TOKEN_XOR_EQ] = "TOKEN_XOR_EQ",
        // KEYWORDS
        [TOKEN_WHILE] = "TOKEN_WHILE",
        [TOKEN_FOR] = "TOKEN_FOR",
        [TOKEN_BREAK] = "TOKEN_BREAK",
        [TOKEN_RETURN] = "TOKEN_RETURN",
        [TOKEN_CONTINUE] = "TOKEN_CONTINUE",
        [TOKEN_IF] = "TOKEN_IF",
        [TOKEN_ELSE] = "TOKEN_ELSE",
        [TOKEN_IMPORT] = "TOKEN_IMPORT",
        [TOKEN_STATIC] = "TOKEN_STATIC",
        [TOKEN_DEFER] = "TOKEN_DEFER",
        [TOKEN_COMPTIME] = "TOKEN_COMPTIME",
        [TOKEN_STRUCT] = "TOKEN_STRUCT",
        [TOKEN_TYPE] = "TOKEN_TYPE",
        [TOKEN_CONST] = "TOKEN_CONST",
        [TOKEN_ENUM] = "TOKEN_ENUM",
        [TOKEN_TRUE] = "TOKEN_TRUE",
        [TOKEN_FALSE] = "TOKEN_FALSE",
        [TOKEN_SWITCH] = "TOKEN_SWITCH",
        [TOKEN_CASE] = "TOKEN_CASE",
        [TOKEN_DEFAULT] = "TOKEN_DEFAULT",
        // TYPES
        [TOKEN_CHAR] = "TOKEN_CHAR",
        [TOKEN_INT] = "TOKEN_INT",
        [TOKEN_FLOAT] = "TOKEN_FLOAT",
        [TOKEN_BOOL] = "TOKEN_BOOL",
        [TOKEN_VOID] = "TOKEN_VOID",
        [TOKEN_NULL] = "TOKEN_NULL",
        // LITERALS
        [TOKEN_LIT_STR] = "TOKEN_LIT_STR",
        [TOKEN_LIT_CHAR] = "TOKEN_LIT_CHAR",
        [TOKEN_LIT_INT] = "TOKEN_LIT_INT",
        [TOKEN_LIT_FLOAT] = "TOKEN_LIT_FLOAT",
        [TOKEN_IDENT] = "TOKEN_IDENT",
        // OTHER
        [TOKEN_EOF] = "TOKEN_EOF",
        [TOKEN_UNKNOWN] = "TOKEN_UNKNOWN",
    };

    if (t.type >= TOKEN_PLUS && t.type < TOKEN_COUNT) {
        Arena a = arena_new(1024);
        LOG(INFO, "%zu:%zu : %s(%s)", t.line, t.col, words[t.type], sstream_to_cstr(&a, t.view));
        return;
    }

    LOG(ERROR, "Token does not exist: %d", t.type);
}

static void print_node_recursive(Node *n) {
    static const char *words[] = {
        // DECLARATIONS
        [NODE_VAR_DECL] = "NODE_VAR_DECL",
        [NODE_FUNC_DECL] = "NODE_FUNC_DECL",
        [NODE_STRUCT_DECL] = "NODE_STRUCT_DECL",
        [NODE_ENUM_DECL] = "NODE_ENUM_DECL",
        [NODE_TYPE_DECL] = "NODE_TYPE_DECL",
        // STATEMENTS
        [NODE_BLOCK] = "NODE_BLOCK",
        [NODE_IF] = "NODE_IF",
        [NODE_WHILE] = "NODE_WHILE",
        [NODE_FOR] = "NODE_FOR",
        [NODE_RETURN] = "NODE_RETURN",
        [NODE_BREAK] = "NODE_BREAK",
        [NODE_CONTINUE] = "NODE_CONTINUE",
        [NODE_DEFER] = "NODE_DEFER",
        [NODE_EXPR_STMT] = "NODE_EXPR_STMT",
        [NODE_IMPORT] = "NODE_IMPORT",
        // EXPRESSIONS
        [NODE_LITERAL] = "NODE_LITERAL",
        [NODE_IDENTIFIER] = "NODE_IDENTIFIER",
        [NODE_BINOP] = "NODE_BINOP",
        [NODE_UNARY] = "NODE_UNARY",
        [NODE_CALL] = "NODE_CALL",
        [NODE_MEMBER_ACCESS] = "NODE_MEMBER_ACCESS",
        [NODE_INDEX] = "NODE_INDEX",
        [NODE_STRUCT_INIT] = "NODE_STRUCT_INIT",
        [NODE_ARRAY_INIT] = "NODE_ARRAY_INIT",
        [NODE_SWITCH] = "NODE_SWITCH",
        [NODE_CASE] = "NODE_CASE",
    };

    if (n->kind >= NODE_VAR_DECL && n->kind < NODE_COUNT) {
        LOG(INFO, "%zu:%zu @ %s : %s", n->line, n->col, n->path, words[n->kind]);

        bool is_one = true;

        // TODO: implement proper printing with names / identifiers / other things
        if (n->next) {
            is_one = false;
            LOG(INFO, "--- Node linked list print start ---");
            print_node_recursive(n->next);
        }

        if (!is_one) {
            LOG(INFO, "--- Node linked list print end ---");
        }

        return;
    }

    LOG(ERROR, "Node does not exist: %d", n->kind);
}

void print_node(Node *n) {
    print_node_recursive(n);
}
