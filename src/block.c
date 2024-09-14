#include "block.h"

#define BLOCK_STATUS_MASK 0b1
#define BLOCK_STATUS_ACTIVE 0b1
#define BLOCK_STATUS_FREE 0b0

#define BLOCK_LENGTH_MASK ~BLOCK_STATUS_MASK


typedef struct _free_block {
    uint64_t state;
    struct _free_block* prev; 
    struct _free_block* next; 
} _free_block_header;

typedef struct {
    _free_block_header* head;
} _free_block_footer;

typedef struct {
    uint64_t state;
} _active_block;

typedef struct {
    _free_block_header* head;
    char mem[];
} *_sa_block_main;


sa_block_allocator sa_block_create() {
    return NULL;
}
sa_block_allocator sa_block_create_e() {
    return NULL;
}
void* sa_block_malloc() {
    return NULL;
}
void* sa_block_calloc() {
    return NULL;
}
void sa_block_free() {
}
void sa_block_destroy() {
}
