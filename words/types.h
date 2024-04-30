#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

typedef struct {
    unsigned int decode: 2;
    unsigned int target_operand: 2;
    unsigned int source_operand: 2;
    unsigned int opcode: 4;
    unsigned int unused: 4;
} CommandWord;

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
    unsigned int target_operand: 3;
    unsigned int source_operand: 3;
    unsigned int unused: 6;
} RegisterWord;


typedef struct {
    /* first Word */
    DirectWord list_word;

    /* second Word */
    InstantWord index_word;
} ConstantIndex;

typedef struct {
    int value: 14;
} DataWord;


typedef union {
    /* opcode */
    CommandWord cmd;

    /* operands */
    InstantWord instant;
    DirectWord direct;
    ConstantIndex constant_index;
    RegisterWord reg;

    /* data word (absolute values) */
    DataWord data;

    /* used to translate the value whatever it may be, and print it, */
    int print: 15;
} Word;

#endif /*ASSEMBLER_TYPES_H*/
