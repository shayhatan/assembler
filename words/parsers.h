/*
 Created by User on 28/02/2024.
*/


#ifndef ASSEMBLER_PARSERS_H
#define ASSEMBLER_PARSERS_H

#include "./types.h"
#include "parsers/parse_types.h"

/* Absolute == instant */
word decodeAbsoluteOperand();

word decodeDirectOperand();

word decodeDirectRegisterOperand();

word decodeRegisterOperand();

word decodeConstantIndexOperand();


word decodeOperands(Operand operand[]);


#endif /*ASSEMBLER_PARSERS_H*/
