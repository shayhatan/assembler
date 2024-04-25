#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./parse_types.h"


bool isInstantAddressing(Operand operand);
bool isValidVariableString(char *word);

String readNextString(char **line, char delimiter, char result_buffer[81]);

Addressing getAddressingFlagForOperand(Operand operand);

int getAddressingForOperand(Operand operand);

ParseResult tryGetOperationWordsCounter(input_line *line, int *words_counter);

bool doesContainLabel(char *line, String *result);

ParseResult tryGetLabelValue(char *line, char **result);

int tryGetDirectiveProps(char *word, enum DirectiveProps *result);

int tryGetOpcode(char *word, enum Opcode *result);

bool isEOF(char *line);

typedef bool (*ValidateArgumentFunction)(char *data);

/* needs de-allocation after finalizing */
ParseResult tryGetArguments(char *line, ArgumentType type, ArgumentsCountType expected_amount, Arguments *args);

int tryGetAssignmentArgument(char *line, DefinitionArgument *argument);

void skipWhitespaces(char **line);

void addArgument(Arguments *args, char arg[DEF_MAX_ARG_CHARS], int arg_index, int arg_size);

#endif /*ASSEMBLER_PARSERS_UTILS_H*/
