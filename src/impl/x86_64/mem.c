#include "mem.h"
#include "print.h"
#include <stddef.h>
#include "io.h"

#define MEM_POOL_SIZE 1024 * 1024 * 64 // 64 MB

typedef struct Block {
    size_t size;
    struct Block* next;
} Block;

static char mem_pool[MEM_POOL_SIZE];
static Block* free_list = (Block*)mem_pool;

void mem_init() {
    free_list->size = MEM_POOL_SIZE;
    free_list->next = NULL;
    
    char mem_pool_size_str[20]; // Buffer to hold the string representation of MEM_POOL_SIZE
    sprintf(mem_pool_size_str, "%d", MEM_POOL_SIZE); // Convert MEM_POOL_SIZE to a string
    print_int(mem_pool_size_str); // Print the string
}

void* malloc(size_t size) {
    Block* prev = NULL;
    Block* block = free_list;
    while (block) {
        if (block->size >= size) {
            if (block->size > size + sizeof(Block)) {
                Block* new_block = (Block*)((char*)block + sizeof(Block) + size);
                new_block->size = block->size - size - sizeof(Block);
                new_block->next = block->next;
                block->size = size;
                block->next = new_block;
            }
            if (prev) {
                prev->next = block->next;
            } else {
                free_list = block->next;
            }
            return (char*)block + sizeof(Block);
        }
        prev = block;
        block = block->next;
    }
    print_str("out of memory\n");
    return NULL;
}

void free(void* ptr) {
    Block* block = (Block*)((char*)ptr - sizeof(Block));
    block->next = free_list;
    free_list = block;
}