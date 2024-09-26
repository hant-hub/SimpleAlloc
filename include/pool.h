#ifndef SA_POOL_H
#define SA_POOL_H
#include "common.h"
#include <stdint.h>

#define BLOCK_ALIGN 8

typedef void* sa_pool_allocator;


extern size_t sa_pool_overhead;

#define sa_pool_stack(name, block_size, block_num) \
    sa_pool_allocator name = sa_pool_create_e( \
            alloca(block_size * block_num + sa_pool_overhead), \
            block_size, \
            block_num)
    
    


sa_pool_allocator sa_pool_create(alloc_func f, uint32_t block_size, uint32_t block_num);
sa_pool_allocator sa_pool_create_e(void* buf, uint32_t block_size, uint32_t block_num);
void* sa_pool_malloc(sa_pool_allocator p);
void* sa_pool_calloc(sa_pool_allocator p);
void sa_pool_free(sa_pool_allocator p, void* ptr);
void sa_pool_destroy(sa_pool_allocator pool, free_func f);
size_t sa_pool_capacity(sa_pool_allocator pool);




#endif
