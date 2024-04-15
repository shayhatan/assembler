#ifndef MMN14_HELPER_H
#define MMN14_HELPER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



typedef void* Pointer;

Pointer allocateMemory(size_t size);

bool isValidMacroName(char* macro_name);


#endif /* MMN14_HELPER_H */
