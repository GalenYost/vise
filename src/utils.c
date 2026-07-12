#include "utils.h"
#include "arena.h"
#include "ast.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
        log_to_console(ERROR, "Could not open file for reading: %s", path);
        log_to_file(ERROR, DEFAULT_LOG_PATH, "Could not open file for reading: %s", path);
        return ss;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize < 0) {
        log_to_console(ERROR, "Could not read file size: %s", path);
        log_to_file(ERROR, DEFAULT_LOG_PATH, "Could not read file size: %s", path);
        fclose(f);
        return ss;
    }

    char *buf = (char*)malloc(fsize + 1);

    if (!buf) {
        log_to_console(FATAL, "Could not allocate %ld bytes to read file %s", fsize, path);
        fclose(f);
        exit(1);
    }

    size_t bytes_read = fread(buf, 1, fsize, f);

    if (bytes_read < (size_t)fsize) {
        log_to_console(ERROR, "Failed to read entire file: %s", path);
        log_to_file(ERROR, DEFAULT_LOG_PATH, "Failed to read entire file: %s", path);
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

#define CONSOLE_PREFIX_FMT "[%s%s%s]: "
#define FILE_PREFIX_FMT "[%s]: "

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

void print_token() {
    static const char *kws[] = {
        // TODO: add other tokens as well
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
        // TYPES
        [TOKEN_STRING] = "TOKEN_STRING",
        [TOKEN_CHAR] = "TOKEN_CHAR",
        [TOKEN_INT] = "TOKEN_INT",
        [TOKEN_FLOAT] = "TOKEN_FLOAT",
        [TOKEN_BOOL] = "TOKEN_BOOL",
        [TOKEN_VOID] = "TOKEN_VOID",
        [TOKEN_NULL] = "TOKEN_NULL",
    };
}
