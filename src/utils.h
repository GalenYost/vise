#pragma once
#include <stdarg.h>
#include <stddef.h>

typedef struct Arena Arena;

typedef struct {
    char *data;
    size_t len;
} StringStream;

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

void log_to_console(LogLevel level, const char *fmt, ...);
void log_to_file(LogLevel level, const char *path, const char *fmt, ...);
