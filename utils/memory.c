#include "./memory.h"
#include "../logs/logging_utils.h"

#define MAX_MEMORY_POOL 4096

int current_allocated_memory = 0;

Pointer allocateMemory(size_t size) {
    Pointer ptr;

/*    if (current_allocated_memory + size >= MAX_MEMORY_POOL) {
        logError("max allowed memory encountered\n");
        return NULL;
    }*/
    ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }

    current_allocated_memory += (int)size;

    return ptr;
}
