# Compiler
CC = gcc
# Compiler flags
CFLAGS = -ansi -pedantic -Wall -g
# Directories
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin
# Executable
TARGET = assembler
LIBS = -lm

${BIN_DIR}/assembler: $(OBJ_DIR)/assembler.o $(OBJ_DIR)/first_run.o $(OBJ_DIR)/second_run.o $(OBJ_DIR)/factory.o $(OBJ_DIR)/map.o $(OBJ_DIR)/labels_table.o $(OBJ_DIR)/logging_utils.o $(OBJ_DIR)/parsers_utils.o $(OBJ_DIR)/string_utils.o $(OBJ_DIR)/line_utils.o $(OBJ_DIR)/decoders.o $(OBJ_DIR)/decoders_table.o $(OBJ_DIR)/macros.o $(OBJ_DIR)/macro_parser.o $(OBJ_DIR)/sorted_list.o $(OBJ_DIR)/precompile.o $(OBJ_DIR)/assembler_run.o $(OBJ_DIR)/assembler_utils.o $(OBJ_DIR)/file_manager.o $(OBJ_DIR)/tables.o $(OBJ_DIR)/externals_table.o $(OBJ_DIR)/memory.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/assembler $(OBJ_DIR)/assembler.o $(OBJ_DIR)/first_run.o $(OBJ_DIR)/second_run.o $(OBJ_DIR)/factory.o $(OBJ_DIR)/map.o $(OBJ_DIR)/labels_table.o $(OBJ_DIR)/logging_utils.o $(OBJ_DIR)/parsers_utils.o $(OBJ_DIR)/string_utils.o $(OBJ_DIR)/line_utils.o $(OBJ_DIR)/decoders.o $(OBJ_DIR)/decoders_table.o $(OBJ_DIR)/macros.o $(OBJ_DIR)/macro_parser.o $(OBJ_DIR)/sorted_list.o $(OBJ_DIR)/precompile.o $(OBJ_DIR)/assembler_run.o $(OBJ_DIR)/assembler_utils.o $(OBJ_DIR)/tables.o $(OBJ_DIR)/file_manager.o $(OBJ_DIR)/externals_table.o $(OBJ_DIR)/memory.o ${LIBS}

# Build rule for assembler.o
$(OBJ_DIR)/assembler.o: assembler.c runs/assembler_run.h runs/assembler_run.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/assembler.o -c assembler.c

# Build rule for factory.o
$(OBJ_DIR)/factory.o: utils/factory.c utils/factory.h parsers/parse_types.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/factory.o -c utils/factory.c

# Build rule for first_run.o
$(OBJ_DIR)/first_run.o: runs/first_run.c runs/first_run.h tables/labels_table.h tables/labels_table.c parsers/parse_types.h parsers/line_utils.h parsers/line_utils.c logs/logging_utils.h logs/logging_utils.c tables/decode_table.h tables/decode_table.c words/decoders.h words/decoders.c utils/string_utils.h utils/string_utils.c parsers/parsers_utils.h parsers/parsers_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/first_run.o -c runs/first_run.c

$(OBJ_DIR)/second_run.o: runs/second_run.c runs/second_run.h runs/assembler_utils.h runs/assembler_utils.c parsers/parse_types.h parsers/line_utils.h parsers/line_utils.c logs/logging_utils.h logs/logging_utils.c words/decoders.h words/decoders.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/second_run.o -c runs/second_run.c

# Build rule for map.o
$(OBJ_DIR)/map.o: data_structures/map/map.c data_structures/map/map.h utils/memory.h utils/memory.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/map.o -c data_structures/map/map.c

# Build rule for string_utils.o
$(OBJ_DIR)/string_utils.o: utils/string_utils.c utils/string_utils.h utils/memory.h utils/memory.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/string_utils.o -c utils/string_utils.c

# Build rule for labels_table.o
$(OBJ_DIR)/labels_table.o: tables/labels_table.c tables/labels_table.h parsers/parse_types.h logs/logging_utils.h logs/logging_utils.c data_structures/map/map.h data_structures/map/map.c utils/factory.h utils/factory.c utils/string_utils.h utils/string_utils.c utils/memory.h utils/memory.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/labels_table.o -c tables/labels_table.c

$(OBJ_DIR)/externals_table.o: tables/externals_table.c tables/externals_table.h data_structures/map/map.h data_structures/map/map.c utils/memory.h utils/memory.c utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/externals_table.o -c tables/externals_table.c

# Build rule for logging_utils.o
$(OBJ_DIR)/logging_utils.o: logs/logging_utils.c logs/logging_utils.h utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/logging_utils.o -c logs/logging_utils.c

$(OBJ_DIR)/memory.o: utils/memory.h utils/memory.c logs/logging_utils.c logs/logging_utils.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/memory.o -c utils/memory.c

# Build rule for parsers_utils.o
$(OBJ_DIR)/parsers_utils.o: parsers/parsers_utils.c parsers/parsers_utils.h parsers/parse_types.h logs/logging_utils.h logs/logging_utils.c words/consts.h utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/parsers_utils.o -c parsers/parsers_utils.c -lm

$(OBJ_DIR)/decoders.o: words/decoders.h words/decoders.c data_structures/map/map.h data_structures/map/map.c parsers/parse_types.h parsers/parsers_utils.h parsers/parsers_utils.c logs/logging_utils.h logs/logging_utils.c words/consts.h utils/string_utils.h utils/string_utils.c tables/labels_table.h tables/labels_table.c tables/decode_table.h tables/decode_table.c tables/externals_table.h tables/externals_table.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/decoders.o -c words/decoders.c

$(OBJ_DIR)/decoders_table.o: tables/decode_table.h tables/decode_table.c data_structures/map/map.h data_structures/map/map.c words/types.h utils/memory.h utils/memory.c utils/string_utils.h utils/string_utils.c tables/labels_table.h tables/labels_table.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/decoders_table.o -c tables/decode_table.c

$(OBJ_DIR)/line_utils.o: parsers/line_utils.h parsers/line_utils.c parsers/parse_types.h parsers/parsers_utils.h parsers/parsers_utils.c logs/logging_utils.h logs/logging_utils.c utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/line_utils.o -c parsers/line_utils.c

$(OBJ_DIR)/macros.o: precompile precompile logs/logging_utils.h logs/logging_utils.c utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/macros.o -c precompile/macro/macros.c

$(OBJ_DIR)/macro_parser.o: precompile precompile logs/logging_utils.h logs/logging_utils.c utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/macro_parser.o -c precompile/macro/macro_parser.c

$(OBJ_DIR)/sorted_list.o: precompile/precompile_list/sorted_list.h precompile/precompile_list/sorted_list.c utils/memory.h utils/memory.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/sorted_list.o -c precompile/precompile_list/sorted_list.c

$(OBJ_DIR)/precompile.o: precompile/precompile.h precompile/precompile.c precompile/macro/macros.h precompile/macro/macros.c precompile/macro/macro_parser.h precompile/macro/macro_parser.h logs/logging_utils.h logs/logging_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/precompile.o -c precompile/precompile.c

$(OBJ_DIR)/assembler_run.o: runs/assembler_run.h runs/assembler_run.c runs/first_run.h runs/first_run.c runs/second_run.h runs/second_run.c utils/file_manager.h utils/file_manager.c precompile/precompile.h precompile/precompile.c logs/logging_utils.h logs/logging_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/assembler_run.o -c runs/assembler_run.c

$(OBJ_DIR)/assembler_utils.o: runs/assembler_utils.h runs/assembler_utils.c utils/memory.h utils/memory.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/assembler_utils.o -c runs/assembler_utils.c

$(OBJ_DIR)/file_manager.o: utils/file_manager.h utils/file_manager.c utils/memory.h utils/memory.c logs/logging_utils.h logs/logging_utils.c utils/string_utils.h utils/string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/file_manager.o -c utils/file_manager.c

$(OBJ_DIR)/tables.o: tables/tables.h tables/tables.c tables/externals_table.h tables/externals_table.c tables/labels_table.h tables/labels_table.c tables/decode_table.h tables/decode_table.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/tables.o -c tables/tables.c

# Default target
all: $(BIN_DIR)/$(TARGET)

# Clean target
clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR)
