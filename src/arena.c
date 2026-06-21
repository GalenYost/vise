#include "arena.h"

void *arena_alloc(Arena *a, size_t size) {
    if (a->offset + size > a->cap) return NULL; 

    void *ptr = a->data + a->offset;
    a->offset += size; 
    
    return ptr;
}

void arena_clear(Arena *a) {
    a->offset = 0;
}
