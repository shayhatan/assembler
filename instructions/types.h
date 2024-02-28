//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

#include "../operands/types.h"

/* A token represents a structural representation of a transformed operand */
typedef struct {
    operand *source;
    operand *target;
    opcode opcode;
} instruction;

/* A structural representation of a parsed instruction */
typedef struct {
    int tokensCount;
    token[] tokens;
} parsedInstruction;

#endif //ASSEMBLER_TYPES_H
