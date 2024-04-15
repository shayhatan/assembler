CC = gcc
CFLAGS = -g -ansi -Wall -pedantic
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# List of source files
SRCS = \
    main.c \
    file_manger/file_manger.c \
    helper/helper.c \
    precomplie/precompile.c \
    precomplie/precompile_list/sorted_list.c \
    precomplie/macro/macro_parser.c \
    precomplie/macro/macros.c

# List of object files
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# Target executable
TARGET = $(BINDIR)/executable

# Main target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

# Create directories
$(OBJDIR) $(BINDIR):
	mkdir -p $@

# Clean rule
clean:
	rm -rf $(OBJDIR) $(BINDIR)
