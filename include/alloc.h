#ifndef SA_ALLOC_H
#define SA_ALLOC_H
#include "stdlib.h"

typedef struct {
    void* allocator;
    void* (*malloc)(void*, size_t);
    void (*free)(void*, void*);
}sa_allocator;

typedef void*(*alloc_func)(size_t);
typedef void(*free_func)(void*);



#endif
