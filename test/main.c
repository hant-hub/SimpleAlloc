#include <stdlib.h>
#include "arena.h"


void fake_free(void* a) {
}


int main() {
    void* buf = malloc(101 * sizeof(int));
    sa_arena a = sa_arena_create_e(buf, 101 * sizeof(int));

    
    int* top = sa_arena_calloc(a, sizeof(int), 1);
    *top = 0;
    for (int i = 0; i < 50; i++) {
        int* b = sa_arena_calloc(a, sizeof(int), 1);
        *b = i;
    }
    for (int i = 50; i < 100; i++) {
        sa_arena_calloc(a, sizeof(int), 1);
    }

    for (int i = 0; i < 101; i++) {
        printf("%d\n", top[i]);
    }

    sa_arena_destroy(fake_free, a);

    return 0;
}
