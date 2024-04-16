
#include "helper.h"
#include "../../logs/logging_utils.h"
#include <string.h>


#define PRE_INSTRUCTION_SIZE 4
#define PRE_CMD_SIZE 16
#define PRE_REG_SIZE 8
#define MAX_MEMORY_POOL 4000

int current_allocated_memory = 0;


Pointer allocateMemory(size_t size) {
    Pointer ptr;

    if (current_allocated_memory + size >= MAX_MEMORY_POOL) {
        logError("max allowed memory encountered");
        return NULL;
    }
    ptr = malloc(size);
    if (ptr == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return NULL;
    }

    current_allocated_memory += (int)size;

    return ptr;
}



bool isInArray(char* str,  char *array[], int size) {
    int i;

    if (str == NULL || array == NULL) {
        return false; /* Return false if either the string or the array is NULL*/
    }

    for (i = 0; i < size; i++) {
        if (strcmp(str, array[i]) == 0) {
            return true; /* Return true if the string is found in the array */
        }
    }
    return false; /* Return false if the string is not found in the array */
}




bool isInstruction(char *str) {
    char *inst[] = {".data", ".string", ".extern", ".entry"};
    return isInArray(str, inst, PRE_INSTRUCTION_SIZE);
}

bool isCmd(char *str) {
    char *cmd[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea",
                   "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts",
                   "hlt"};

    return isInArray(str, cmd, PRE_CMD_SIZE);

}

bool isRegister(char* str) {
    char* regs[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
    return isInArray(str, regs, PRE_REG_SIZE);
}


bool isValidMacroName(char* macro_name) {
    return !(isRegister(macro_name) || isCmd(macro_name) || isInstruction(macro_name));
}

