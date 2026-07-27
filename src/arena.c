#include "arena.h"

#include <stdlib.h>
#include <string.h>

Arena arena_new(size_t cap) {
    Arena a = {0};
    if (cap == 0) cap = 1024;
    a.data = (uint8_t *)malloc(cap);
    a.cap = a.data ? cap : 0;
    a.offset = 0;
    return a;
}

void *arena_alloc(Arena *a, size_t size) {
    if (a->offset + size > a->cap) {
        size_t new_cap = a->cap ? a->cap * 2 : 1024;
        while (new_cap < a->offset + size) new_cap *= 2;
        uint8_t *new_data = (uint8_t *)realloc(a->data, new_cap);
        if (!new_data) return NULL;
        a->data = new_data;
        a->cap = new_cap;
    }

    void *ptr = a->data + a->offset;
    a->offset += size;
    return ptr;
}

void arena_clear(Arena *a) {
    a->offset = 0;
}

void arena_free(Arena *a) {
    free(a->data);
    a->data = NULL;
    a->cap = 0;
    a->offset = 0;
}
