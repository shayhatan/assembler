//
// Created by User on 20/03/2024.
//

#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./types.h"


enum Addressing getAddressingForOperand(Operand operand);

int getOperationWordsCounter(input_line *line);


enum SentenceType getSentenceType(char *line);

bool doesContainLabel(char *line);

/* needs de-allocation after finalizing */
char *getLabelValue(char *line);

enum DirectiveProps getDirectiveProps(char *line);


enum opcode getOpcode(char *line);

/* needs de-allocation after finalizing */
List getArguments(char *line, enum ArgumentType type, enum ArgumentsCountType expectedAmount);

/* needs de-allocation after finalizing */
Arguments *getOperationArguments(char *line);

#endif //ASSEMBLER_PARSERS_UTILS_H
