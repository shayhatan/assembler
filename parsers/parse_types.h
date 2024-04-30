
#ifndef ASSEMBLER_PARSERS_TYPES_H
#define ASSEMBLER_PARSERS_TYPES_H

#include <stdbool.h>
#include "../words/consts.h"

typedef enum Addressing {
    INSTANT = 1,
    DIRECT = 2,
/*  indirectRegister = 4, */
    DIRECT_REGISTER = 8,
    CONSTANT_INDEX = 16
} Addressing;

typedef enum DirectiveProps {
    DOT_DATA = 1, DOT_STRING = 2, DOT_EXTERNAL = 4, DOT_ENTRY = 8, DOT_DEFINE = 16
} DirectiveProps;

typedef char *Operand;


typedef struct {
    char *constant_id;
    int constant_value;
} DefinitionArgument;

/* symbol properties (symbol-table metadata) */
#define DEF_DOT_CODE ".code"
#define DEF_DOT_DATA ".data"
#define DEF_DOT_EXTERNAL ".external"
#define DEF_DOT_DEFINE ".mdefine"

/* raw directives */
#define DEF_RAW_DOT_STRING ".string"
#define DEF_RAW_DOT_DATA ".data"
#define DEF_RAW_DOT_EXTERN ".extern"
#define DEF_MAX_ARG_CHARS 36
#define DEF_MAX_ARGS 80

typedef enum ParseResult {
    PARSE_SUCCESS, PARSE_FAILURE, OUT_OF_MEMORY
} ParseResult;

typedef struct {
    char *classification; /* symbol property */
    int value; /* IC + 100 || DC || constant value */
    unsigned int words_counter;
    bool is_entry;
} Entry;

typedef struct {
    char *content;
    int size;
} String;


typedef struct {
    /* given that a line size is capped at a certain size, we can limit the size of the arguments it may define, and thus avoid allocations */
    char args[DEF_MAX_ARGS][DEF_MAX_ARG_CHARS];
    int args_count;
} Arguments;

typedef struct {
    bool isEOF;
    bool has_label; /* does begin with a symbol */
    char *label;
    Arguments arguments;
    DefinitionArgument const_definition_arg;
    enum DirectiveProps directive_props;
    enum Opcode opcode;
    int line_number;
    bool is_comment;
    bool is_empty;
} InputLine;

typedef enum ArgumentType {
    NUMERIC_TYPE, STRING_TYPE, DOUBLE_QUOTE_STRING, LABEL_TYPE
} ArgumentType;

typedef enum ArgumentsCountType {
    ANY = -3, SINGLE = -2, PLURAL = -1 /* we pass an integer (exact amount) or these enum values as expected amount */
} ArgumentsCountType;


#endif /*ASSEMBLER_PARSERS_TYPES_H*/
