#ifndef SA_BLOCK_H
#define SA_BLOCK_H
#include "common.h"
#include <stdint.h>


typedef void* sa_block_allocator;

extern size_t sa_block_overhead;

sa_block_allocator sa_block_create(alloc_func f, size_t size);
sa_block_allocator sa_block_create_e(void* buf, size_t size);
void* sa_block_malloc(sa_block_allocator a, size_t size);
void* sa_block_calloc(sa_block_allocator a, size_t num, size_t elem);
void sa_block_free(sa_block_allocator b, void* ptr);
void sa_block_destroy(sa_block_allocator a, free_func f);



#endif
