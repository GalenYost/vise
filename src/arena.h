#pragma once
#include <stdint.h>
#include "utils.h"

typedef struct Arena {
    uint8_t *data;
    size_t cap;
    size_t offset;
} Arena;

void *arena_alloc(Arena *a, size_t size);
void arena_clear(Arena *a);
