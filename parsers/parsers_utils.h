#ifndef ASSEMBLER_PARSERS_UTILS_H
#define ASSEMBLER_PARSERS_UTILS_H

#include "./parse_types.h"


/**
 * Determines if the given operand uses instant addressing.
 *
 * @param operand The operand to check.
 * @return True if instant addressing is used, false otherwise.
 */
bool isInstantAddressing(Operand operand);

/**
 * Checks if the given string is a valid variable string.
 *
 * @param word The string to validate.
 * @return True if the string is a valid variable string, false otherwise.
 */
bool isValidVariableString(char *word);

/**
 * Reads the next string from the input line until the specified delimiter.
 *
 * @param line The input line to read from.
 * @param delimiter The delimiter to stop at.
 * @param result_buffer The buffer to store the result string.
 * @return The string read from the input line.
 */
String readNextString(char **line, char delimiter, char result_buffer[81]);

/**
 * Retrieves the number of operands required by the given operation.
 *
 * @param code The operation code.
 * @return The number of operands required.
 */
int getAmountOfOperandsByOperation(enum Opcode code);

/**
 * Retrieves the addressing flag for the given operand.
 *
 * @param operand The operand to get the addressing flag for.
 * @return The addressing flag.
 */
Addressing getAddressingFlagForOperand(Operand operand);

/**
 * Retrieves the addressing mode for the given operand.
 *
 * @param operand The operand to get the addressing mode for.
 * @return The addressing mode.
 */
int getAddressingForOperand(Operand operand);

/**
 * Tries to obtain the count of words related to an operation from the input line.
 *
 * @param line The input line.
 * @param words_counter Pointer to store the count of words.
 * @return ParseResult indicating success or failure.
 */
ParseResult tryGetOperationWordsCounter(InputLine *line, int *words_counter);

/**
 * Checks if the given line contains a label and extracts it.
 *
 * @param line The input line to check.
 * @param result Pointer to store the extracted label.
 * @return True if a label is found, false otherwise.
 */
bool doesContainLabel(char *line, String *result);

/**
 * Attempts to retrieve the value of a label from the given line.
 *
 * @param line The input line.
 * @param result Pointer to store the label value.
 * @return ParseResult indicating success or failure.
 */
ParseResult tryGetLabelValue(char *line, char **result);

/**
 * Tries to get properties of a directive.
 *
 * @param word The directive word.
 * @param result Pointer to store the directive properties.
 * @return The number of directive properties obtained.
 */
int tryGetDirectiveProps(char *word, enum DirectiveProps *result);

/**
 * Tries to retrieve the opcode from the given word.
 *
 * @param word The word to parse.
 * @param result Pointer to store the opcode.
 * @return The number of opcode obtained.
 */
int tryGetOpcode(char *word, enum Opcode *result);

/**
 * Checks if the given line represents the end of the file.
 *
 * @param line The input line to check.
 * @return True if end of file, false otherwise.
 */
bool isEOF(char *line);

<<<<<<< HEAD
/* needs de-allocation after finalizing */
=======
/**
 * Tries to parse arguments from the given line according to the specified type and expected amount.
 *
 * @param line The input line to parse.
 * @param type The type of arguments.
 * @param expected_amount The expected number of arguments.
 * @param args Pointer to store the parsed arguments.
 * @return ParseResult indicating success or failure.
 */
>>>>>>> 9d3d75962876230096dbaeba75c2e97ee85e6d80
ParseResult tryGetArguments(char *line, ArgumentType type, ArgumentsCountType expected_amount, Arguments *args);

/**
 * Tries to retrieve an assignment argument from the given line.
 *
 * @param line The input line to parse.
 * @param argument Pointer to store the assignment argument.
 * @return The number of assignment arguments obtained.
 */
int tryGetAssignmentArgument(char *line, DefinitionArgument *argument);

/**
 * Skips any leading whitespace characters in the line.
 *
 * @param line Pointer to the input line.
 */
void skipWhitespaces(char **line);

/**
 * Adds an argument to the argument list along with its index and size.
 *
 * @param args Pointer to the arguments list.
 * @param arg The argument to add.
 * @param arg_index The index of the argument.
 * @param arg_size The size of the argument.
 */
void addArgument(Arguments *args, char arg[DEF_MAX_ARG_CHARS], int arg_index, int arg_size);

#endif /*ASSEMBLER_PARSERS_UTILS_H*/
