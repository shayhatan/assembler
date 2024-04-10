
#ifndef MMN14_LABEL_H
#define MMN14_LABEL_H






/**
 * Represents a label's address and associated information.
 */
typedef struct label {
    int address;
    char *label_name;
    int assembly_line;
    int is_data_line;
} Label;




#endif /*MMN14_LABEL_H*/
