/*
 Created by User on 28/02/2024.
*/

#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

typedef struct {
    unsigned int decode: 2;
    unsigned int targetOperand: 2;
    unsigned int sourceOperand: 2;
    unsigned int opcode: 4;
    unsigned int unused: 4;
} command_word;

typedef struct {
    unsigned int decode: 2;
    int value: 12;
} InstantWord;

typedef struct {
    unsigned int decode: 2;
    unsigned int value: 12;
} DirectWord;


typedef struct {
    unsigned int decode: 2;
    unsigned int targetOperand: 3;
    unsigned int sourceOperand: 3;
    unsigned int unused: 6;
} register_word;


typedef struct {
    /* first word */
    DirectWord list_word;

    /* second word */
    InstantWord index_word;
} ConstantIndex;

typedef struct {
    int value: 14;
} DataWord;


typedef union {
    /* opcode */
    command_word cmd;

    /* operands */
    InstantWord instant;
    DirectWord direct;
    ConstantIndex constant_index;
    register_word reg;

    /* data */
    DataWord data;

    /* used to translate the value whatever it may be, and print it, */
    int print: 15;
} word;

#endif /*ASSEMBLER_TYPES_H*/
