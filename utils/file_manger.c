#include "file_manger.h"
#include "memory.h"
#include <stdlib.h>
#include <string.h>

/* Function to check if a character followed by a space is a comma-space pair */
bool isCommaSpacePair(char current, char next) {
    return (current == ',' && next == ' '); /* Check if current character is a comma and next character is a space */
}

/* Function to check if a space followed by a character is a space-comma pair */
bool isSpaceCommaPair(char current, char next) {
    return (current == ' ' && next == ','); /* Check if current character is a space and next character is a comma */
}


/* Function to compact comma-space pairs into single commas */
void compactCommas(char *input) {
    int i, j = 0;

    /* Loop through the input string */
    for (i = 0; input[i] != '\0'; i++) {
        /* If current character and next character form a comma-space pair */
        if (isCommaSpacePair(input[i], input[i + 1])) {
            input[j++] = input[i++]; /* Copy current character */
        }
            /* If current character and next character form a space-comma pair, skip the space */
        else if (isSpaceCommaPair(input[i], input[i + 1])) {
            continue;
        }
            /* Otherwise, copy current character */
        else {
            input[j++] = input[i];
        }
    }

    input[j] = '\0'; /* Null-terminate the modified string */
}


/* Function to format a string */
void formatString(char *input) {
    removeExcessSpaces(input); /* Remove excess spaces */
    compactCommas(input); /* Compact comma-space pairs */
}




/* Function to check if a line exceeds the maximum allowed length */
bool isLineTooLong(const char *line) {
    size_t len = strlen(line); /* Get the length of the line */

    /* Check if the length exceeds the maximum allowed length minus 1 (for null terminator)
       and if the character at the maximum allowed length is not a newline */
    return (len >= PRE_MAX_LINE - 1 && line[PRE_MAX_LINE - 2] != '\n');
}


/* Function to copy text from a file */
char *copyTextFile(FILE *fp, fpos_t *pos, int length) {
    char *str = allocateMemory(length + 1); /* Allocate memory for the string */

    if (!str) {
        return NULL;
    }

    if (fsetpos(fp, pos) != 0) { /* Set file position indicator to the given position */
        printf("fsetpos in copyTextFile failed\n");
        free(str); /* Free allocated memory */
        return NULL;
    }

    /* Read text from file into the allocated memory */
    fread(str, sizeof(char), length, fp);
    str[length] = '\0'; /* Null-terminate the string */

    fgetpos(fp, pos); /* Get current file position */

    return str; /* Return the copied string */
}


/* Function to change the extension of a file name */
void changeFileExtension(char *file_name, const char *new_extension) {
    char *dot = strrchr(file_name, '.'); /* Find the last occurrence of '.' in file_name */

    if (dot != NULL) { /* If extension exists */
        strcpy(dot, new_extension); /* Replace existing extension with new_extension */
    } else { /* If no extension exists */
        strcat(file_name, new_extension); /* Append new_extension to file_name */
    }
}

/* Function to generate an output file name based on the input file name and extension */
void generateOutputFileName(const char *input_file_name, char *output_file_name, const char *extension) {
    strcpy(output_file_name, input_file_name); /* Copy input_file_name to output_file_name */
    changeFileExtension(output_file_name, extension); /* Change extension of output_file_name */
}

/**
 * Function to format a file.
 * Returns true if formatting is successful, false otherwise.
 */
bool formatFile(const char *input_file_name, char *new_file1) {
    FILE *input_file;
    FILE *output_file;
    char line[PRE_MAX_LINE];
    line[PRE_MAX_LINE - 1] = '\0'; /* Null-terminate the line buffer */

    /* Generate output file name */
    generateOutputFileName(input_file_name, new_file1, ".out");

    /* Open input file */
    input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        new_file1[0] = '\0'; /* Empty the new_file1 array if an error occurs */
        return false;
    }

    /* Open output file */
    output_file = fopen(new_file1, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        fclose(input_file);
        new_file1[0] = '\0';
        return false;
    }

    /* Read each line from input file, format it, and write to output file */
    while (fgets(line, sizeof(line), input_file)) {
        size_t length;
        if (isLineTooLong(line)) {
            fprintf(stderr, "Error: Line too long in input file\n");
            fclose(input_file);
            fclose(output_file);
            return false;
        }
        formatString(line); /* Format the line */
        /* Skip blank lines */ /* Do we want to skip? @itay*/
        length = strlen(line);
        if (length > 0)
            fprintf(output_file, "%s\n", line); /* Write formatted line to output file */
    }

    /* Close the files */
    fclose(input_file);
    fclose(output_file);
    return true;
}


/* Function to check if a file name ends with ".am" */
bool endsWithAM(const char *file_name) {
    size_t len = strlen(file_name); // Get the length of the file name

    // Check if the file name is at least 3 characters long
    if (len >= 3) {
        // Compare the last three characters of the file name with ".am"
        return (strcmp(file_name + len - 3, ".am") == 0);
    } else {
        // File name is too short to end with ".am"
        return false;
    }
}