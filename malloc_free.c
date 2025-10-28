#include <stdio.h>
#include <stdint.h>
#include<stddef.h>

#define Heap_size 1024 * 64 //64 KB heap

typedef struct blockheader
{
    size_t size;
    int free;
    struct blockheader *next;
} blockheader;

static uint8_t heap[Heap_size];
static blockheader *free_list = NULL;

void heap_init(void);
void *my_malloc(size_t size);
void my_free(void *ptr);
void dump_heap(void);
void split_block(blockheader *block, size_t size);

int main(void)
{
    heap_init();
    printf("Initial:\n");
    dump_heap();

    void *a = my_malloc(200);
    void *b = my_malloc(500);
    printf("After two allocations:\n");
    dump_heap();

    my_free(a);
    printf("After freeing first block:\n");
    dump_heap();

    void *c = my_malloc(100);
    printf("After reallocating 100 bytes:\n");
    dump_heap();

    (void)b;
    (void)c;
    return 0;
}

void heap_init(void)
{
    free_list = (blockheader *)heap;
    free_list->size = Heap_size - sizeof(blockheader);
    free_list->free = 1;
    free_list->next = NULL;
}

void split_block(blockheader *block, size_t size) {
    // check if we can create a new free block from leftover space
    if (block->size >= size + sizeof(blockheader) + 8) {
        blockheader *new_block = (blockheader *)((uint8_t *)block +
                                  sizeof(blockheader) + size);
        new_block->size = block->size - size - sizeof(blockheader);
        new_block->free = 1;
        new_block->next = block->next;

        block->size = size;
        block->next = new_block;
    }
}

void *my_malloc(size_t size) {
    if (size == 0) return NULL;

    blockheader *curr = free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {
            split_block(curr, size);
            curr->free = 0;
            return (uint8_t *)curr + sizeof(blockheader);
        }
        curr = curr->next;
    }

    // no suitable block
    return NULL;
}

void my_free(void *ptr) {
    if (ptr == NULL) return;

    blockheader *block = (blockheader *)((uint8_t *)ptr - sizeof(blockheader));
    block->free = 1;
}

// ---- Debug helper ----
void dump_heap(void) {
    printf("Heap layout:\n");
    blockheader *curr = free_list;
    int i = 0;
    while (curr) {
        printf("  [%02d] addr=%p | size=%zu | %s\n",
               i++, (void *)curr, curr->size,
               curr->free ? "FREE" : "USED");
        curr = curr->next;
    }
    printf("\n");
}
