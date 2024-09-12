#include "arena.h"
#include <string.h>
#include <stddef.h>

typedef struct {
    uint32_t top;
    uint32_t capacity;
    char mem[];
} _sa_arena_direct;

typedef _sa_arena_direct* _sa_arena;


void* sa_arena_create(alloc_func allocator, size_t cap) {
    _sa_arena a = (*allocator)(cap + offsetof(_sa_arena_direct, mem));
    if (!a) {
        return NULL;
    }
    a->top = 0;
    a->capacity = cap;
    return a;
}
void* sa_arena_create_e(void* buf, size_t cap) {
    _sa_arena out = buf;
    out->top = 0;
    out->capacity = cap;
    return out;
}
void* sa_arena_malloc(sa_arena a, size_t size){ 
    _sa_arena held = a;
    uint32_t top = held->top + size;
    uint32_t out = held->top;
    if (top > held->capacity)
        return NULL;
    held->top += size;
    return held->mem + out;
}
void* sa_arena_calloc(sa_arena a, size_t num, size_t elem) {
    _sa_arena held = a;
    uint32_t size = num * elem;
    
    uint32_t top = held->top + size;
    uint32_t out = held->top;
    if (top > held->capacity)
        return NULL;
    held->top += size;

    for (int i = 0; i < size; i++) {
        held->mem[out + i] = 0;
    }

    return held->mem + out;
}
void sa_arena_destroy(free_func allocator, sa_arena a) {
    _sa_arena held = a;
    (*allocator)(held);
}
