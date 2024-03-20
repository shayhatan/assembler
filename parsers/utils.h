//
// Created by User on 20/03/2024.
//

#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./types.h"


enum Addressing getAddressingForOperand(Operand operand);

int getOperationWordsCounter(input_line *line);

#endif //ASSEMBLER_PARSERS_UTILS_H
