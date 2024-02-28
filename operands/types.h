//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_TYPES_H
#define ASSEMBLER_TYPES_H

enum operandDefinition {
    Absolute,
    Relative,
    External
};

enum operandType {
    source,
    target
};

enum operandDataType {
    integer,
    character,
    boolean
};

typedef struct {
    operandType type;
    operandDefinition definition;
    operandDataType dataType;
    /* alternatively we can use int, char, bool pointers */
    void *data;
} operand;


/* Encapsulates a parsed token and relevant metadata (f.i - how many operands does it contain, the original operands e.t.c) */
/* at least one of source or target should be properly defined */
typedef struct {
    operand* source;
    operand* target;
    /* parsed content in binary */
    char[14] content;
} token;

#endif //ASSEMBLER_TYPES_H
