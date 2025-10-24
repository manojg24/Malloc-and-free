#include <stdio.h>
#include <stdint.h>
#include<stddef.h>

#define Heap_size 1024 * 64 //64 KB heap

int main(void)
{
    
}

typedef struct blockheader
{
    size_t size;
    int free;
    struct blockheader *next;
} blockheader;

static uint8_t heap[Heap_size];
static blockheader *free_list = NULL;

void heap_main(void)
{
    free_list = (blockheader *)heap;
    free_list->size = Heap_size - sizeof(blockheader);
    free_list->free = 1;
    free_list->next = NULL;
}

