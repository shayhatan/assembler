
#ifndef ASSEMBLER_PARSERS_TYPES_H
#define ASSEMBLER_PARSERS_TYPES_H

#include <stdbool.h>
#include "../words/consts.h"

enum Addressing {
    instant = 1,
    direct = 2,
/*  indirectRegister = 4, */
    directRegister = 8,
    constantIndex = 16
};

enum DirectiveProps {
    dot_data = 1, dot_string = 2, dot_external = 4, dot_entry = 8, dot_define = 16
};

typedef char *Operand;


typedef struct {
    char *constant_id;
    int constant_value;
} DefinitionArgument;

/* symbol properties (symbol-table metadata) */
#define DOT_CODE ".code"
#define DOT_DATA ".data"
#define DOT_EXTERNAL ".external"
#define DOT_DEFINE ".mdefine"

/* raw directives */
#define RAW_DOT_STRING ".string"
#define RAW_DOT_DATA ".data"
#define RAW_DOT_EXTERN ".extern"
#define MAX_ARG_CHARS 36
#define MAX_ARGS 80

typedef enum ParseResult {
    PARSE_SUCCESS, PARSE_FAILURE, OUT_OF_MEMORY
} ParseResult;

typedef struct {
    char *classification; /* symbol property */
    int value; /* IC + 100 || DC || constant value */
    unsigned int wordsCounter;
    bool isEntry;
} entry;


typedef struct {
    char *content;
    int size;
} String;



typedef struct {
    /* given that a line size is capped at a certain size, we can limit the size of the arguments it may define, and thus avoid allocations */
    char args[MAX_ARGS][MAX_ARG_CHARS];
    int args_count;
} Arguments;

typedef struct {
    bool isEOF;
    bool hasLabel; /* does begin with a symbol */
    char *label;
    Arguments arguments;
    DefinitionArgument const_definition_arg;
    enum DirectiveProps directive_props;
    enum Opcode opcode;
    int lineNumber;
    bool isComment;
    bool isEmpty;
} input_line;

enum SentenceType {
    EMPTY, COMMENT, DIRECTIVE, INSTRUCTION, CONSTANT_DEFINITION,
    /* reserved for collisions or invalid categorization during parsing */
    INVALID
};

enum ArgumentType {
    NUMERIC_TYPE, STRING_TYPE, DOUBLE_QUOTE_STRING, LABEL_TYPE
};

enum ArgumentsCountType {
    ANY, SINGLE, PLURAL
};


#endif /*ASSEMBLER_PARSERS_TYPES_H*/
