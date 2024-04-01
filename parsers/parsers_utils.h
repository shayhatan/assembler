//
// Created by User on 20/03/2024.
//

#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./parse_types.h"

bool isNumber(char *word);

String readNextString(char **line, char delimiter, char result_buffer[81]);

enum Addressing getAddressingForOperand(Operand operand);

enum ParseResult tryGetOperationWordsCounter(input_line *line, int *words_counter);

bool doesContainLabel(char *line, String *result);

enum ParseResult tryGetLabelValue(char *line, char **result);

int tryGetDirectiveProps(char *word, enum DirectiveProps *result);

int tryGetOpcode(char *word, enum opcode *result);

bool isEOF(char *line);

typedef bool (*ValidateArgumentFunction)(char *data);

/* needs de-allocation after finalizing */
enum ParseResult
tryGetArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount, Arguments *args);

int tryGetAssignmentArgument(char *line, DefinitionArgument *argument);

void skipWhitespaces(char **line);

void addArgument(Arguments *args, char arg[MAX_ARG_CHARS], int arg_index, int arg_size);

#endif //ASSEMBLER_PARSERS_UTILS_H
