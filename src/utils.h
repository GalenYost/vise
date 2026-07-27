#pragma once

#include <stdarg.h>
#include <stddef.h>

typedef struct {
    char *data;
    size_t len;
} StringStream;

typedef struct Arena Arena;
typedef struct Token Token;
typedef struct Node Node;

StringStream sstream_new(char *str);

StringStream sstream_dump(Arena *a, StringStream ss);
char *sstream_to_cstr(Arena *a, StringStream ss);

StringStream read_file(const char *path);

// logging
typedef enum {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
} LogLevel;

#define DEFAULT_LOG_PATH "build.log"

void log_to_console(LogLevel level, const char *fmt, ...);
void log_to_file(LogLevel level, const char *path, const char *fmt, ...);

#define LOG(level, ...) \
    do { \
        log_to_console(level, __VA_ARGS__); \
        log_to_file(level, DEFAULT_LOG_PATH, __VA_ARGS__); \
    } while (0)

void print_token(Token t);
void print_node(Node *n);
