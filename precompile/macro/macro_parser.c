#include "macro_parser.h"
#include "../../utils/string_utils.h"
#include "../../logs/logging_utils.h"


/**************************** static functions ***************************************/
/* Function to check if the line is not "endmcro" */
static int isNotEndMcro(const char *str) {
    return strcmp(str, "endmcr\n") != 0;
}

/* Function to check for unexpected text after "endmcr" */
static int hasUnexpectedText(const char *str) {
    return (strstr(str, "endmcr") != NULL && strlen(str) != strlen("endmcr\n"));
}

/**
 * Function to read macro content from file.
 * Returns a pointer to the macro content if successful, NULL otherwise.
 */
char *readMacroData(FILE *fp, fpos_t *pos, int *line_count, _Bool *parse_failure) {
    int mcr_len = 0; /* Variable to store the length of macro content */
    char str[PRE_MAX_LINE] = ""; /* Buffer to store each line of the file */
    char *data; /* Pointer to store the macro content */

    /* Set file position indicator to the given position */
    if (fsetpos(fp, pos) != 0) {
        printf("fsetpos in read_macro_content failed\n");
        return NULL;
    }

    /* Read lines until reaching the end of the macro */
    while (fgets(str, PRE_MAX_LINE, fp) != NULL && isNotEndMcro(str)) {
        (*line_count)++; /* Increment line count */
        mcr_len += strlen(str); /* Accumulate length of macro content */
        /* Check for unexpected text after 'endmcro' */
        if (hasUnexpectedText(str)) {
            printf("Error: Unexpected text after 'endmcro'\n");
            *parse_failure = true;
            return NULL;
        }
        memset(str, '\0', PRE_MAX_LINE); /* Clear the buffer */
    }

    /* Copy macro content from file */
    data = copyTextFile(fp, pos, mcr_len);
    return data;
}


/**
 * @brief Parses the formatted file, extracts macros, and inserts them into the Macros struct.
 *
 * This function reads the formatted file line by line, identifying macro definitions and extracting their content.
 * It then inserts the macros into the provided Macros struct. Macro definitions are expected to be in a specific format.
 *
 * @param macros A pointer to the Macros struct where the extracted macros will be stored.
 * @param formatted_file_name The name of the formatted file containing macro definitions.
 * @return bool Returns true if the parsing and insertion process is successful, false otherwise.
 */
_Bool processMacroLines(Macros *macros, const char *formatted_file_name, bool *parse_failure) {
    FILE *fp = fopen(formatted_file_name, "r");
    char line[PRE_MAX_LINE] = "";
    char mcr_name[PRE_MAX_LINE] = "";
    int cnt = 0;
    char* data;
    fpos_t pos;

    if (fp == NULL) {
        logError("Error opening formatted file\n");
        return false;
    }

    /* file is already formatted hence lines size are valid*/
    while (fgets(line, sizeof(line), fp)) {
        cnt++;
        /* if there is error here it's not fatal meaning that file we will continue to check more erros*/
        if (processAddMcrLine(line, cnt, mcr_name, parse_failure)) {
            /* Get current file position. */
            fgetpos(fp, &pos);
            data = readMacroData(fp, &pos, &cnt, parse_failure);

            if (data) {
                fsetpos(fp, &pos);
                insertMacro(macros, mcr_name, data);
            }

        }
        memset(line, '\0', PRE_MAX_LINE);
    }
    fclose(fp);
    return !parse_failure;
}


bool isLastLineWord(const char *token) {
    return (token != NULL && strtok(NULL, " ") == NULL);
}


/**
 * Function to process a line of code when adding a macro definition.
 * Returns true if processing is successful, false otherwise.
 */
bool processAddMcrLine(char *line, int line_number, char *name, bool *error) {
    char *token;
    token = strtok(line, " \n"); /* Tokenize the line using space and newline as delimiters */

    /* Check if the first token is "mcr" */
    if (token != NULL && strcmp(token, "mcr") == 0) {
        token = strtok(NULL, " \n"); /* Get the next token */

        /* Check if the token is the last Word in the line */
        if (isLastLineWord(token)) {
            /* Clear the name buffer */
            memset(name, '\0', PRE_MAX_LINE);
            /* Copy the token (macro name) to the name buffer */
            strcpy(name, token);

            /* Check if the macro name is valid */
            if (isValidMacroName(name)) {
                return true; /* Return true if macro name is valid */
            }
            logError("Invalid macro name! (reg/inst etc)"); /* Print error message if macro name is invalid */
        }
            /* If token is NULL, it means there was no name provided */
        else if (token == NULL) {
            logError("Error in line %d: Invalid format\n", line_number); /* Print error message */
        } else {
            logError("Error in line %d: Extra text after macro name definition\n",
                     line_number); /* Print error message */
        }
        /**error = true;*/ /* Set error flag to true */
    }
    return false; /* Return false by default */
}


/* Function to replace occurrences of macro_name with allocated_data in a file */
bool replaceMacrosInFile(const char *filename, Macros *macros, char *am_file, bool parse_failure) {
    FILE *destination_file;
    char destination_base[PRE_MAX_LINE] = "";
    char line[PRE_MAX_LINE] = "";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return false;
    }

    if (parse_failure) {
        return true;
    }

    generateOutputFileName(filename, destination_base, ".am");

    /* Open destination file */
    destination_file = fopen(destination_base, "w");
    if (destination_file == NULL) {
        printf("Error opening destination file %s\n", destination_base);
        fclose(file);
        return false;
    }

    strcpy(am_file, destination_base); /* Copy destination file name to am_file */
    printf("===========Am File==================\n");
    while (fgets(line, PRE_MAX_LINE, file) != NULL) {
        Node *current = macros->macros->head;
        while (current != NULL) {
            Macro *macro = (Macro *) current->data;
            char *pos = strstr(line, macro->macro_name); /* Find occurrence of macro_name in line */
            while (pos != NULL) {
                int index = pos - line; /* Position of macro_name in line*/
                /* Replace macro_name with allocated_data*/
                strncpy(&line[index], macro->allocated_data, strlen(macro->allocated_data));
                pos = strstr(line + index + strlen(macro->allocated_data), macro->macro_name);
            }
            current = current->next;
        }
        printf("%s", line);/* Print or write the modified line to output file */
        fprintf(destination_file, "%s", line);
        memset(line, '\0', PRE_MAX_LINE); /* Clear line buffer */
    }

    fclose(file);
    fclose(destination_file);
    return true;
}


/**
 * Function to check if a given buffer indicates the start of a macro block.
 * Returns true if the buffer contains the start of a macro block, false otherwise.
 */
bool isMacroBlockStart(const char *buffer, Node *macros_head, bool *in_macro_block) {
    Node *current = macros_head; /* Pointer to traverse the list of macros */
    while (current != NULL) {
        Macro *macro = (Macro *) current->data; /* Get the current macro */
        char *start_macro_pos = strstr(buffer, "mcr "); /* Find occurrence of "mcr " in buffer */
        if (start_macro_pos != NULL) { /* If "mcr " is found */
            char *macro_name_start = start_macro_pos + strlen("mcr "); /* Get start position of macro name */
            char *macro_name_end = strchr(macro_name_start, '\n'); /* Find end position of macro name */
            if (macro_name_end != NULL) { /* If end of macro name is found */
                *macro_name_end = '\0'; /* Null terminate the macro name */
                if (strcmp(macro_name_start, macro->macro_name) ==
                    0) { /* Compare macro name with current macro's name */
                    *in_macro_block = true; /* Set in_macro_block flag to true */
                    return true; /* Return true indicating the start of a macro block */
                }
            }
        }
        current = current->next; /* Move to the next macro in the list */
    }
    return false; /* Return false if no macro block start is found */
}


/**
 * Function to remove macro blocks from a source file and save the result to a destination file.
 * Returns true if removal is successful, false otherwise.
 */
bool removeMacros(const char *source_filename, char *destination_filename, Macros *macros) {
    char buffer[PRE_MAX_LINE]; /* Buffer to store each line of the source file */
    bool in_macro_block = false; /* Flag to track if currently inside a macro block */
    FILE *destination_file;
    char destination_base[PRE_MAX_LINE];

    /* Open source file */
    FILE *source_file = fopen(source_filename, "r");
    if (source_file == NULL) {
        printf("Error opening source file %s\n", source_filename);
        return false;
    }

    /* Generate output file name */
    generateOutputFileName(source_filename, destination_base, ".out1");

    /* Open destination file */
    destination_file = fopen(destination_base, "w");
    if (destination_file == NULL) {
        printf("Error opening destination file %s\n", destination_base);
        fclose(source_file);
        return false;
    }

    strcpy(destination_filename, destination_base); /* Copy destination file name to destination_filename */

    /* Process each line of the source file */
    while (fgets(buffer, PRE_MAX_LINE, source_file) != NULL) {
        if (in_macro_block) { /* If inside a macro block */
            if (strstr(buffer, "endmcr") != NULL) { /* Check if the end of the macro block is reached */
                in_macro_block = false; /* Set in_macro_block flag to false */
            }
            continue; /* Skip copying this line */
        }

        if (!isMacroBlockStart(buffer, macros->macros->head, &in_macro_block)) { /* Check if a macro block starts */
            fprintf(destination_file, "%s", buffer); /* Copy the line to the destination file */
        }
    }

    fclose(source_file);
    fclose(destination_file);

    return true;
}




