# Compiler
CC = gcc
# Compiler flags
CFLAGS = -ansi -pedantic -Wall
# Directories
SRC_DIR = .
OBJ_DIR = ./obj
BIN_DIR = ./bin
# Executable
TARGET = assembler
LIBS = -lm

${BIN_DIR}/assembler: $(OBJ_DIR)/assembler.o $(OBJ_DIR)/first_run.o $(OBJ_DIR)/factory.o $(OBJ_DIR)/map.o $(OBJ_DIR)/labels_table.o $(OBJ_DIR)/logging_utils.o $(OBJ_DIR)/parsers_utils.o $(OBJ_DIR)/string_utils.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/assembler $(OBJ_DIR)/assembler.o $(OBJ_DIR)/first_run.o $(OBJ_DIR)/factory.o $(OBJ_DIR)/map.o $(OBJ_DIR)/labels_table.o $(OBJ_DIR)/logging_utils.o $(OBJ_DIR)/parsers_utils.o $(OBJ_DIR)/string_utils.o ${LIBS}

# Build rule for assembler.o
$(OBJ_DIR)/assembler.o: assembler.c first_run.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/assembler.o -c assembler.c

# Build rule for factory.o
$(OBJ_DIR)/factory.o: factory.c factory.h parsers/parse_types.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/factory.o -c factory.c

# Build rule for first_run.o
$(OBJ_DIR)/first_run.o: first_run.c first_run.h labels_table.h labels_table.c parsers/parse_types.h logs/logging_utils.h logs/logging_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/first_run.o -c first_run.c

# Build rule for map.o
$(OBJ_DIR)/map.o: data_structures/map/map.c data_structures/map/map.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/map.o -c data_structures/map/map.c

# Build rule for string_utils.o
$(OBJ_DIR)/string_utils.o: string_utils.c string_utils.h
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/string_utils.o -c string_utils.c

# Build rule for labels_table.o
$(OBJ_DIR)/labels_table.o: labels_table.c labels_table.h parsers/parse_types.h logs/logging_utils.h logs/logging_utils.c data_structures/map/map.h data_structures/map/map.c factory.h factory.c string_utils.h string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/labels_table.o -c labels_table.c

# Build rule for logging_utils.o
$(OBJ_DIR)/logging_utils.o: logs/logging_utils.c logs/logging_utils.h string_utils.h string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/logging_utils.o -c logs/logging_utils.c

# Build rule for parsers_utils.o
$(OBJ_DIR)/parsers_utils.o: parsers/parsers_utils.c parsers/parsers_utils.h parsers/parse_types.h logs/logging_utils.h logs/logging_utils.c words/consts.h string_utils.h string_utils.c
	$(CC) $(CFLAGS) -o $(OBJ_DIR)/parsers_utils.o -c parsers/parsers_utils.c -lm


# Default target
all: $(BIN_DIR)/$(TARGET)

# Clean target
clean:
	$(RM) -r $(OBJ_DIR) $(BIN_DIR)
