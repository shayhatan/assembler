//
// Created by User on 17/03/2024.
//

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "./parsers/parse_types.h"
#include "labels_table.h"
#include "./data_structures/linked_list/list.h"
#include "logs/logging_utils.h"
#include "factory.h"
#include "parsers/parsers_utils.h"

enum ParseResult {
    PARSE_FAILURE, SUCCESS
};

enum analyze_status {
    STOP, NEXT
};


int run(FILE *srcFile);