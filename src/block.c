#include "block.h"

typedef struct {
    uint32_t size;
    size_t next_block;
} block_header;


typedef struct {
    block_header* head;
}* _sa_block;
