#include "./memory.h"
#include "../logs/logging_utils.h"

#define MAX_MEMORY_POOL 4096

int allocations_counter = 0;

Pointer allocateMemory(size_t size) {
    Pointer ptr;

    ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }

    return ptr;
}

Pointer allocateAndCountMemory(size_t size) {
    Pointer ptr;

    if (allocations_counter + 1 > MAX_MEMORY_POOL) {
        logError("max allowed memory encountered\n");
        return NULL;
    }

    ptr = allocateMemory(size);

    if (ptr != NULL) {
        allocations_counter++;
    }

    return ptr;
}
