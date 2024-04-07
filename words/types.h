/*
 Created by User on 28/02/2024.
*/

#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

#include "../data_structures/linked_list/list.h"


typedef struct {
    unsigned int decode: 2;
    unsigned int targetOperand: 2;
    unsigned int sourceOperand: 2;
    unsigned int opcode: 4;
    unsigned int unused: 4;
} command_word;

typedef struct {
    unsigned int decode: 2;
    unsigned int value: 12;
} argument_word;


typedef struct {
    unsigned int decode: 2;
    unsigned int targetOperand: 3;
    unsigned int sourceOperand: 3;
    unsigned int unused: 6;
} register_word;


typedef union {
    command_word cmd;
    register_word reg;
    argument_word arg;
} word;

/* Encapsulates a parsed token (source word including all of its extra words in that order) */
typedef struct {
    List words;
} instruction;

#endif /*ASSEMBLER_TYPES_H*/
