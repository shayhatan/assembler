#include "given_label.h"
#include <stdlib.h>
#include <string.h>


/*if there is '.extern' we will use this and not 'label' structure */
struct GivenLabel_t {
    char *label_name;
    int assembly_line;
};


/*need to add cmp function dep on which one is the key label_name or assembly_line?
 * in label.c it seems that the key is label_name */

/* assumings args are valid!!! probably checked in parsers file*/
GivenLabel givenLabelCreate(char *label_name, int assembly_line) {
    GivenLabel given_label = malloc(sizeof(*given_label));
    if (!given_label)
        return NULL;

    /* Allocate memory for label_name and copy the provided label_name*/
    given_label->label_name = malloc(strlen(label_name) + 1); /* +1 for the null terminator*/
    if (!given_label->label_name) {
        free(given_label); // Free the memory allocated for the struct
        return NULL;
    }
    strcpy(given_label->label_name, label_name);
    given_label->assembly_line = assembly_line;

    return given_label;
}

GivenLabel givenLabelCopy(GivenLabel given_label) {
    if (!given_label)
        return NULL;

    /* Allocate memory for the new GivenLabel structure */
    GivenLabel copied_label = malloc(sizeof(*copied_label));
    if (!copied_label)
        return NULL;

    /* Allocate memory for the label_name and copy the data */
    copied_label->label_name = malloc(strlen(given_label->label_name) + 1); /* +1 for the null terminator */
    if (!copied_label->label_name) {
        free(copied_label); // Free the memory allocated for the struct
        return NULL;
    }
    strcpy(copied_label->label_name, given_label->label_name);

    /* Copy the assembly_line */
    copied_label->assembly_line = given_label->assembly_line;

    return copied_label;
}


void givenLabelDestroy(GivenLabel given_label) {
    if (given_label == NULL)
        return;

    free(given_label->label_name);
    free(given_label);
}


void *givenLabelCopyWrapper(void *given_label) {
    return (void *) givenLabelCopy((GivenLabel) given_label);
}

void givenLabelDestroyWrapper(void *given_label) {
    givenLabelDestroy((GivenLabel) given_label);
}

char* getGivenLabelName(GivenLabel given_label) {
    if  (!given_label)
        return NULL;
    return given_label->label_name;
}

int getGivenLabelLine(GivenLabel given_label) {
    if  (!given_label)
        return EXIT_FAILURE;

    return given_label->assembly_line;
}




