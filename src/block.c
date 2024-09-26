#include "block.h"
#include "common.h"

#define BLOCK_LENGTH_MASK ~BLOCK_STATUS_MASK
#define BLOCK_ALIGN _Alignof(long)


typedef struct _free_block {
    uint64_t size;
    struct _free_block* prev; 
    struct _free_block* next; 
    char mem[];
} header;

typedef struct {
    uint64_t state;
} active;

typedef struct _sa_main{
    header* head;
    char mem[];
} *block_main;

size_t sa_block_overhead = sizeof(struct _sa_main) + sizeof(struct _free_block);

sa_block_allocator sa_block_create(alloc_func f, size_t size) {
    size += sa_block_overhead;
    size += size % BLOCK_ALIGN;

    block_main a = f(size);

    a->head = (header*)a->mem;
    *(a->head) = (header) {
        .next = NULL,
        .prev = NULL,
        .size = size - sa_block_overhead
    };
    printf("full size: %ld\n", size - sa_block_overhead);

    return a;
}
sa_block_allocator sa_block_create_e() {
    return NULL;
}
void* sa_block_malloc(sa_block_allocator a, size_t size) {
    //size += sa_block_overhead;
    size += size % BLOCK_ALIGN;
    printf("in size: %ld\n", size);

    block_main m = a;
    header* head = m->head;

    header* curr = head;
    header* found = NULL;

    while (curr) {
        if (curr->size >= size) {
            found = curr;
            break;
        }
        curr = curr->next;
    }

    if (!found) return NULL;
    
    //split block
    if (found->size - sizeof(header) > size) {
        header* new = (header*)&found->mem[size];
        *new = (header) {
            .size = found->size - (size + sizeof(header)),
            .prev = found->prev,
            .next = found->next
        };
        found->size = size;
        printf("out size: %ld\n", found->size);

        if (found->next) found->next->prev = new;
        if (found->prev) found->prev->next = new;
        else m->head = new;
        return ((active*)found) + 1;
    }

    if (found->next) found->next->prev = found->prev;
    if (found->prev) found->prev->next = found->next;
    else m->head = found->next;

    return ((active*)found) + 1;
}
void* sa_block_calloc() {
    return NULL;
}
void sa_block_free(sa_block_allocator b, void* ptr) {
    block_main m = b;
    header* freed = (header*)((active*)ptr - 1);

    header* found = NULL;
    header* prev = NULL;
    header* curr = m->head;

    while (curr) {
        if (curr > freed) {
            found = curr->prev;
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    //insert

    if (!found && !curr) {
        //reached end without finding space to insert
        prev->next = freed;
        freed->prev = prev;
        freed->next = NULL;
    } else if (!found && curr) {
        //freed block preceeds head of list
        curr->prev = freed;
        freed->next = curr;
        freed->prev = NULL;
        m->head = freed;
    } else if (found && curr) {
        //freed block is in the middle of list
        freed->next = curr;
        freed->prev = found;
        curr->prev = freed;
        prev->next = freed;
    }

    //merge

    printf("unmerged: %ld\n", freed->size);

    header* merged = NULL;

    //check next
    if (freed->next) {
        header* next = freed->next;
        if ((header*)&freed->mem[freed->size] == next) {
            freed->next = next->next;
            freed->size += sizeof(header) + next->size;
            merged = freed;
        }
    }

    //check prev
    if (freed->prev) {
        header* prev = freed->prev;
        if ((header*)&prev->mem[prev->size] == freed) {
            prev->next = freed->next;
            prev->size += sizeof(header) + freed->size;
            merged = prev;
        }
    }

    if (merged) {
        if (merged->next) merged->next->prev = merged;
        if (merged->prev) merged->prev->next = merged;
        else m->head = merged;
        printf("merged: %ld\n", merged->size);
    }
}
void sa_block_destroy(sa_block_allocator a, free_func f) {
    f(a);
}
