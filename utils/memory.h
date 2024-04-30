#ifndef MMN14_HELPER_H
#define MMN14_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef void* Pointer;

/** Allocate dynamic memory without counting the allocation */
Pointer allocateMemory(size_t size);
/** Allocate dynamic memory and count the allocation */
Pointer allocateAndCountMemory(size_t size);

#endif /* MMN14_HELPER_H */
