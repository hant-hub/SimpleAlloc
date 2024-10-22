#ifndef SA_ARENA_H
#define SA_ARENA_H
#include <stdint.h>
#include "common.h"


typedef void* sa_arena;
extern size_t sa_arena_overhead;

#define sa_scratch_arena(name, cap) \
    sa_arena name = alloca(2 * sizeof(uint32_t) + cap); \
    ((uint32_t*)name)[0] = 0; \
    ((uint32_t*)name)[1] = cap


sa_arena sa_arena_create(alloc_func allocator, size_t cap);
sa_arena sa_arena_create_e(void* buf, size_t cap);
void* sa_arena_malloc(sa_arena a, size_t size);
void* sa_arena_calloc(sa_arena a, size_t num, size_t elem);
void sa_arena_reset(sa_arena a);
size_t sa_arena_capacity(sa_arena a);
void sa_arena_destroy(free_func allocator, sa_arena a);



#endif
