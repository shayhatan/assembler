cmake_minimum_required(VERSION 3.26)
project(assembler C)

set(CMAKE_C_STANDARD 90)

# Add compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ansi -pedantic -Wall -g")

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

include_directories(.)
include_directories(data_structures/linked_list)
include_directories(data_structures/map)

add_executable(assembler
        words/consts.h
        words/parsers.h
        words/types.h
        runs/second_run.c
        runs/first_run.c
        data_structures/map/map.c
        utils/factory.h
        tables/labels_table.c
        tables/labels_table.h
        parsers/parsers_utils.h
        parsers/parsers_utils.c
        assembler.c
        utils/factory.c
        logs/logging_utils.c
        utils/string_utils.c
        utils/string_utils.h
        words/decoders.c
        tables/decode_table.h
        tables/decode_table.c
        words/decoders.h
        parsers/line_utils.c
        parsers/line_utils.h
        runs/second_run.h
        utils/file_manger.c
        utils/memory.c
        tables/externals_table.c
        tables/externals_table.h
        precomplie/precompile.c
        precomplie/precompile.h
        precomplie/macro/macro_parser.c
        precomplie/macro/macro_parser.h
        precomplie/macro/macros.c
        precomplie/macro/macros.c
        precomplie/precompile_list/sorted_list.c
        precomplie/precompile_list/sorted_list.h
        runs/assembler_run.c
        runs/assembler_run.h
        tables/tables.c
        tables/tables.h
        runs/assembler_types.c
        runs/assembler_types.h
)

# Link math library
target_link_libraries(assembler m)
