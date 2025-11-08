#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h>

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
blockheader *find_free_block(size_t size);
void my_free(void *ptr);
void coalesce_free_blocks(void);
void dump_heap(void);
void split_block(blockheader *block, size_t size);
void benchmark_allocator(void);
void test_fragmentation_pattern(void);
void test_stress_pattern(void);

void calc_heap_stats(size_t *total_free, size_t *total_used, int *free_blocks, int *used_blocks) {
    *total_free = 0;
    *total_used = 0;
    *free_blocks = 0;
    *used_blocks = 0;

    blockheader *curr = free_list;
    while (curr) {
        if (curr->free) {
            *total_free += curr->size;
            (*free_blocks)++;
        } else {
            *total_used += curr->size;
            (*used_blocks)++;
        }
        curr = curr->next;
    }
}

// Calculate fragmentation ratio
double calc_fragmentation_ratio(void) {
    size_t total_free = 0, total_used = 0;
    int free_blocks = 0, used_blocks = 0;
    calc_heap_stats(&total_free, &total_used, &free_blocks, &used_blocks);

    if (total_free == 0) return 0.0;

    // Find largest free block
    size_t largest_free = 0;
    blockheader *curr = free_list;
    while (curr) {
        if (curr->free && curr->size > largest_free)
            largest_free = curr->size;
        curr = curr->next;
    }

    return 1.0 - ((double)largest_free / (double)total_free);
}

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

    void *c = my_malloc(400);
    printf("After reallocating 100 bytes:\n");
    dump_heap();

    (void)b;
    (void)c;
    my_free(b);
    my_free(a);
    dump_heap();

    test_fragmentation_pattern();
    test_stress_pattern();
    benchmark_allocator();

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

blockheader *find_free_block(size_t size)
{
    blockheader *curr = free_list;
    while (curr) {
        if (curr->free && curr->size >= size)
            return curr;
        curr = curr->next;
    }
    return NULL;
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
    coalesce_free_blocks();
}

void coalesce_free_blocks(void)
{
    blockheader *curr = free_list;
    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += sizeof(blockheader) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
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

void test_fragmentation_pattern(void) {
    printf("=== Fragmentation Test ===\n");
    void *ptrs[10];

    // Allocate several blocks
    for (int i = 0; i < 10; i++) {
        ptrs[i] = my_malloc(200);
    }

    // Free some in between to create gaps
    for (int i = 0; i < 10; i += 2) {
        my_free(ptrs[i]);
    }

    dump_heap();

    double frag = calc_fragmentation_ratio();
    printf("Fragmentation Ratio: %.2f%%\n\n", frag * 100);
}

void test_stress_pattern(void) {
    printf("=== Stress Test ===\n");
    void *ptrs[500];
    srand(42);

    for (int i = 0; i < 500; i++) {
        size_t sz = rand() % 256 + 16; // random sizes
        ptrs[i] = my_malloc(sz);

        // randomly free
        if (rand() % 4 == 0 && ptrs[i]) {
            my_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }

    dump_heap();
    double frag = calc_fragmentation_ratio();
    printf("Post-stress fragmentation: %.2f%%\n\n", frag * 100);
}

void benchmark_allocator(void) {
    printf("=== Performance Benchmark ===\n");

    const int N = 10000;
    void *ptrs[N];

    clock_t start = clock();

    for (int i = 0; i < N; i++) {
        ptrs[i] = my_malloc(32);
        if (i % 3 == 0 && ptrs[i]) my_free(ptrs[i]);
    }

    clock_t end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Performed %d allocations/frees in %.3f seconds.\n\n", N, time_taken);
}
