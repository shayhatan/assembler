//
// Created by User on 20/03/2024.
//

#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./parse_types.h"

bool isNumber(char *word);

/* mutates line */
/* returns a dynamically allocated sub-string copy of n chars */
char *readNextString(char **line, char delimiter);

enum Addressing getAddressingForOperand(Operand operand);

int getOperationWordsCounter(input_line *line);

bool doesContainLabel(char *line);

/* needs de-allocation after finalizing */
char *getLabelValue(char *line);

int tryGetDirectiveProps(char *word, enum DirectiveProps *result);

int tryGetOpcode(char *word, enum opcode *result);

bool isEOF(char *line);

typedef bool (*ValidateArgumentFunction)(char *data);

/* needs de-allocation after finalizing */
int tryGetArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount, List args);

int tryGetAssignmentArgument(char *line, DefinitionArgument *argument);

void skipWhitespaces(char **line);

#endif //ASSEMBLER_PARSERS_UTILS_H
