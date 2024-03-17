//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_PARSERS_H
#define ASSEMBLER_PARSERS_H

#include "./types.h"

/* Absolute == instant */
word parseAbsoluteOperand();

word parseDirectOperand();

word parseDirectRegisterOperand();

word parseRegisterOperand();

word parseConstantIndexOperand();


#endif //ASSEMBLER_PARSERS_H
