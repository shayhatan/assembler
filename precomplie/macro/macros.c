#include "macros.h"
#include "../../utils/string_utils.h"
#include "logs/logging_utils.h"

int compareMacroNames(const void *a, const void *b) {
    const Macro *macro_a = (const Macro *) a;
    const Macro *macro_b = (const Macro *) b;
    return strcmp(macro_a->macro_name, macro_b->macro_name);
}

void freeMacroData(void *data) {
    Macro *macro = (Macro *) data;
    free(macro->macro_name);
    free(macro->allocated_data);
    free(macro);
}

Macros *createMacros() {
    Macros *macros = (Macros *) allocateMemory(sizeof(Macros));
    if (macros != NULL) {
        macros->macros = initializeList(compareMacroNames, freeMacroData);
    }
    return macros;
}

void freeMacros(Macros *macros) {
    if (macros != NULL) {
        freeList(macros->macros);
        free(macros);
    }
}


void insertMacro(Macros *macros, const char *macro_name, char *data) {
    char *mcr_name;
    Macro *macro;
    if (macros != NULL && macro_name != NULL && data != NULL) {
        mcr_name = allocatedDuplicateString(macro_name);
        macro = createMacro(mcr_name, data);
        if (macro != NULL) {
            insert(macros->macros, macro);
        } else {
            logError("Memory allocation failed.\n");
        }
    }
}

/* Function to get the data associated with a macro */
char *getMacroData(Macros *macros, const char *macro_name) {
    if (macros != NULL && macro_name != NULL) {
        Node *current = macros->macros->head;
        while (current != NULL) {
            Macro *macro = (Macro *) current->data;
            if (strcmp(macro->macro_name, macro_name) == 0) {
                return macro->allocated_data;
            }
            current = current->next;
        }
    }
    return NULL;
}

/* Function to check if the Macros struct contains a specific macro */
bool isContainMacro(Macros *macros, const char *macro_name) {
    if (macros != NULL && macro_name != NULL) {
        Node *current = macros->macros->head;
        while (current != NULL) {
            Macro *macro = (Macro *) current->data;
            if (strcmp(macro->macro_name, macro_name) == 0) {
                return true;
            }
            current = current->next;
        }
    }
    return false;
}

Macro *createMacro(char *macro_name, char *data) {
    Macro *macro = (Macro *) allocateMemory(sizeof(Macro)); /* malloc(sizeof(Macro)); */
    if (macro != NULL) {
        macro->macro_name = macro_name;
        macro->allocated_data = data; /* Assuming data is already allocated */
    }
    return macro;
}


char *getReplacedName(char *line, const char *data, const char *name) {
    char *found = strstr(line, name); /* Find the occurrence of the name in the line */
    size_t prefix_length;
    size_t suffix_length;
    size_t new_line_length;
    char *new_line;
    if (found != NULL) { /* If the name is found */

        /* Null-terminate the string by replacing the last character ('\n') with '\0' */
        *(line + strlen(line) - 1) = '\0';

        /* Calculate the length of the parts before and after the name*/
        prefix_length = found - line;
        suffix_length = strlen(found + strlen(name));

        /* Allocate memory for the new line */
        new_line_length = prefix_length + strlen(data) + suffix_length + 1;
        new_line = (char *) allocateMemory(new_line_length); /* malloc(new_line_length); */

        if (!new_line) {
            return NULL;
        }

        /* Copy the prefix part of the line */
        memcpy(new_line, line, prefix_length);
        new_line[prefix_length] = '\0'; /* Null-terminate the string */

        /* Concatenate the data in place of the name */
        strcat(new_line, data);

        /* Concatenate the suffix part of the line */
        strcat(new_line, found + strlen(name));

        /* Copy the modified line back to the original line */
        return new_line;
    }
    return NULL;
}


void replaceMacros(const char *file_name, char *out_put, Macros *macros) {
    FILE *input_file = fopen(file_name, "r");
    FILE *output_file;
    char line[PRE_MAX_LINE] = "";
    char *new_line = NULL; /* To store modified line*/
    if (input_file == NULL) {
        logError( "Error opening file %s\n", file_name);
        return;
    }

    output_file = fopen(out_put, "w");
    if (output_file == NULL) {
        logError("Error creating temporary output file\n");
        fclose(input_file);
        return;
    }

    while (fgets(line, sizeof(line), input_file) != NULL) {
        /* Iterate over each macro and replace occurrences in the line */
        Node *current = macros->macros->head;
        while (current != NULL) {
            Macro *macro = (Macro *) current->data;
            char *replaced_line = getReplacedName(line, macro->allocated_data, macro->macro_name);
            if (replaced_line != NULL) {
                if (new_line != NULL) {
                    free(new_line); /* Free previous memory before assigning new value */
                }
                new_line = replaced_line;
                break; /* No need to check other macros for this line */
            }
            current = current->next;
        }

        if (new_line != NULL) {
            /*  Write the modified line to the output file */
            fprintf(output_file, "%s", new_line);
            free(new_line); /* Free memory allocated for new_line */
            new_line = NULL;
        } else {
            /* Write the original line to the output file */
            fprintf(output_file, "%s", line);
        }
    }

    fclose(input_file);
    fclose(output_file);

}
