#ifndef ASSEMBLER_EXTERNALS_TABLE_H
#define ASSEMBLER_EXTERNALS_TABLE_H

#include "../data_structures/map/map.h"
#include <stdio.h>


/**
 * Disposes of the memory allocated for an externals map.
 *
 * @param externals_map The externals map to dispose of.
 */
void externalsMapDispose(Map externals_map);

/**
 * Adds an external label to the externals map at the specified address.
 *
 * @param address The address of the external label.
 * @param label The external label to add.
 * @param externals_map The externals map to add the label to.
 * @return MapResult indicating success or failure.
 */
MapResult addExternal(int address, char *label, Map externals_map);

/**
 * Checks if the externals map is empty.
 *
 * @param externals_map The externals map to check.
 * @return True if the externals map is empty, false otherwise.
 */
bool isEmptyExternals(Map externals_map);

/**
 * Writes the externals map to an output file.
 *
 * @param ext_file The output file pointer.
 * @param externals_map The externals map to write.
 * @return The number of externals written to the file.
 */
int writeExternals(FILE *ext_file, Map externals_map);

/**
 * Creates a new externals map.
 *
 * @return The newly created externals map.
 */
Map externalsMapCreate(void);

#endif /*ASSEMBLER_EXTERNALS_TABLE_H*/
