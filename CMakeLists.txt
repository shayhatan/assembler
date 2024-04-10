cmake_minimum_required(VERSION 3.26)
project(assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(data_structures/linked_list)
include_directories(data_structures/table)
include_directories(files)
include_directories(words)

add_executable(assembler
        data_structures/linked_list/types.h
        data_structures/linked_list/utils.c
        data_structures/linked_list/utils.h
        data_structures/table/types.h
        data_structures/table/utils.c
        data_structures/table/utils.h
        files/utils.h
        words/consts.h
        words/parsers.h
        words/types.h
        consts.h
        second_run.c)
