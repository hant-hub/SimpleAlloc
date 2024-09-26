#include <stdlib.h>
#include "arena.h"
#include "pool.h"
#include "block.h"


void fake_free(void* a) {
}


int main() {

    /*
    //arena test
    void* buf = malloc(101 * sizeof(int) + sa_arena_overhead);
    sa_arena a = sa_arena_create_e(buf, 101 * sizeof(int));

    
    int* top = sa_arena_calloc(a, sizeof(int), 1);
    *top = 0;
    for (int i = 0; i < 50; i++) {
        int* b = sa_arena_calloc(a, sizeof(int), 1);
        printf("arena cap = %ld\n", sa_arena_capacity(a));
        *b = i;
    }
    sa_arena_reset(a);
    for (int i = 50; i < 100; i++) {
        sa_arena_calloc(a, sizeof(int), 1);
        printf("arena cap = %ld\n", sa_arena_capacity(a));
    }

    //for (int i = 0; i < 101; i++) {
    //    printf("%d\n", top[i]);
    //}

    sa_arena_destroy(free, a);


    //pool test
    //sa_pool_allocator p = sa_pool_create(malloc, 16, 10); 
    //void* mem = malloc(16 * 10 + sa_pool_overhead);
    //sa_pool_allocator p = sa_pool_create_e(mem, 16, 10);
    
    sa_pool_stack(p, 16, 10);
    void* ptrs[3];
    for (int i = 0; i < 3; i++) {
        printf("alloc 0x%lx\n", (unsigned long)(ptrs[i] = sa_pool_malloc(p)));
    }

    
    for (int i = 0; i < 3; i++) {
        printf("freed 0x%lx\n", (unsigned long)ptrs[i]);
        sa_pool_free(p, ptrs[i]);
    }

    for (int i = 0; i < 3; i++) {
        printf("calloc 0x%lx\n", (unsigned long)(ptrs[i] = sa_pool_calloc(p)));
        for (int j = 0; j < 16; j++) {
            printf("%x", ((char*)ptrs[i])[j]);
        }
        printf("\n");
    }
    printf("%ld\n", sa_pool_capacity(p));
    */


    sa_block_allocator b = sa_block_create(malloc, 100000);

    char* strings[10];
    for (int i = 0; i < 10; i++) {
        strings[i] = sa_block_malloc(b, 1);
        strings[i][0] = i;
    }

    for (int i = 0; i < 10; i++) {
        printf("%d", strings[i][0]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++) {
        sa_block_free(b, strings[i]);
    }


    void* t = sa_block_malloc(b, 100000);
    void* w = sa_block_malloc(b, 1);
    printf("t: 0x%lx\nw: 0x%lx\n", (long)t, (long)w);

    sa_block_destroy(b, free);



    return 0;
}
