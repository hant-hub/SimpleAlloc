#include "pool.h"
#include "common.h"
#include <stdlib.h>

typedef struct _sa_block {
    struct _sa_block* next; 
} _sa_block;

typedef struct _sa_pool_direct {
    uint32_t block_size;
    uint32_t block_num;
    _sa_block* head;  
    _sa_block mem;
} *_sa_pool;


size_t sa_pool_overhead = sizeof(struct _sa_pool_direct) - sizeof(_sa_block);


sa_pool_allocator sa_pool_create(alloc_func f, uint32_t block_size, uint32_t block_num) {
    //kinda ugly but we take those
    block_size += (block_size % BLOCK_ALIGN) != 0;

    _sa_pool pool = (*f)(block_size * (block_num) + sa_pool_overhead);
    pool->block_size = block_size;
    pool->block_num = block_num;

    _sa_block* curr = &pool->mem; 
    _sa_block* next = (_sa_block*)(((char*)curr) + block_size);
    block_num--;
    pool->head = curr;
    for (int i = 0; i < block_num; i++) {
        curr->next = next;  
        curr = next;
        next = (_sa_block*)(((char*)curr) + block_size);
    }

    curr->next = NULL;
    return pool;
}
sa_pool_allocator sa_pool_create_e(void* buf, uint32_t block_size, uint32_t block_num) {
    //kinda ugly but we take those
    block_size += (block_size % BLOCK_ALIGN) != 0;

    _sa_pool pool = buf;
    pool->block_size = block_size;
    pool->block_num = block_num;

    _sa_block* curr = &pool->mem; 
    _sa_block* next = (_sa_block*)(((char*)curr) + block_size);
    block_num--;
    pool->head = curr;
    for (int i = 0; i < block_num; i++) {
        curr->next = next;  
        curr = next;
        next = (_sa_block*)(((char*)curr) + block_size);
    }

    curr->next = NULL;
    return pool;
}
void* sa_pool_malloc(sa_pool_allocator p) {
    _sa_pool pool = p;
    _sa_block* alloc = pool->head;
    pool->head = alloc->next;
    return alloc;
}
void* sa_pool_calloc(sa_pool_allocator p ) {
    _sa_pool pool = p;
    _sa_block* alloc = pool->head;
    pool->head = alloc->next;
    for (int i = 0; i < pool->block_size; i++) {
        ((char*)alloc)[i] = 0;
    }
    return alloc;
}
void sa_pool_free(sa_pool_allocator p, void* ptr) {
   _sa_pool pool = p; 
   _sa_block* freed = ptr;
    freed->next = pool->head;
    pool->head = freed;
}
void sa_pool_destroy(sa_pool_allocator pool, free_func f) {
    (*f)(pool);
}

size_t sa_pool_capacity(sa_pool_allocator pool) {
    size_t out = 0;
    _sa_pool p = pool;
    _sa_block* curr = p->head; 

    while(curr) {
        out++;
        curr = curr->next;
    }

    return out * p->block_size;
}
