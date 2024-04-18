#include <stdio.h>
#include <stdbool.h>
#include "../tables/labels_table.h"
#include "../parsers/line_utils.h"
#include "../logs/logging_utils.h"
#include "../words/decoders.h"
#include "../utils/string_utils.h"


static int address = 100;

static enum ParseResult analyzeLine(input_line *line) {
    MapResult status;
    if (line->directive_props & (dot_data | dot_string | dot_external | dot_define))
    {
        return PARSE_SUCCESS;
    }

    if (line->directive_props & dot_entry) {
        status = setEntryLabel(line->arguments.args[0]);
        if (status != MAP_SUCCESS) {
            logError("Failed to set %s as entry label", line->label);
            return status == MAP_OUT_OF_MEMORY ? OUT_OF_MEMORY : PARSE_FAILURE;
        }
        return PARSE_SUCCESS;
    }

    status = decodeInstruction(&address, line->opcode, line->arguments.args, line->arguments.args_count);

    switch (status) {

        case MAP_SUCCESS:
            return PARSE_SUCCESS;
        case MAP_ERROR:
        case MAP_NULL_ARGUMENT:
        case MAP_ITEM_ALREADY_EXISTS:
        case MAP_ITEM_DOES_NOT_EXIST:
            logError("failed to analyze line\n");
            return PARSE_FAILURE;
        case MAP_OUT_OF_MEMORY:
            logError("out of memory\n");
            return OUT_OF_MEMORY;
    }

    return PARSE_SUCCESS;
}

enum ParseResult secondRun(FILE *srcFile) {
    char buffer[81];
    int index = 0;

    while (fgets(buffer, 81, srcFile) != 0) {
        input_line line;
        enum ParseResult parse_result;
        bool shouldStop = false;

        removeExcessSpaces(buffer);

        resetLine(&line);

        setLogLineContext(index, buffer);

        parse_result = parseLine(buffer, index++, &line);

        switch (parse_result) {
            case PARSE_FAILURE:
                logError("Failed to parse line %d %s\n", index - 1, buffer);
                disposeLine(&line);
                continue;
            case OUT_OF_MEMORY:
                logError("gracefully clearing all allocations and shutting down\n");
                disposeLine(&line);
                labelsTableDispose();
                return OUT_OF_MEMORY; /* complete bail out */
            case PARSE_SUCCESS: /* do nothing */
                break;
        }


        if (line.isComment || line.isEmpty) {
            disposeLine(&line);
            continue;
        }

        switch (analyzeLine(&line)) {
            case PARSE_SUCCESS:
                break;
            case PARSE_FAILURE:
                shouldStop = true;
                break;
            case OUT_OF_MEMORY:
                shouldStop = true;
                logError("Out of memory!\n");
                break;
        }
        disposeLine(&line);
        if (shouldStop) {
            break;
        }
    }

    if (errored) {
        return PARSE_FAILURE; /* custom error code */
    }

    return PARSE_SUCCESS;
}
