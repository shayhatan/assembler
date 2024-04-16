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
        consts.h
        second_run.c
        first_run.c
        data_structures/map/map.c
        factory.h
        labels_table.c
        labels_table.h
        parsers/parsers_utils.h
        parsers/parsers_utils.c
        assembler.c
        factory.c
        logs/logging_utils.c
        string_utils.c
        string_utils.h
        words/decoders.c
        decode_table.h
        decode_table.c
        words/decoders.h
        parsers/line_utils.c
        parsers/line_utils.h
        second_run.h
        preprocessor/file_manger/file_manger.c
        preprocessor/helper/helper.c
)

# Link math library
target_link_libraries(assembler m)
