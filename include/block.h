#ifndef SA_BLOCK_H
#define SA_BLOCK_H
#include <stdint.h>
#include <stdlib.h>

typedef void* sa_block_allocator;

extern size_t sa_block_overhead;


sa_block_allocator sa_block_create();
sa_block_allocator sa_block_create_e();
void* sa_block_malloc();
void* sa_block_calloc();
void sa_block_free();
void sa_block_destroy();



#endif
