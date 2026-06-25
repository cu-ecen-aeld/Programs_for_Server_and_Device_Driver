#------------------------------------------------------------------------------
# Makefile
#
# Build configuration for the writer application, which creates any
# required directories and writes a specified string to a file.
#
# Supports native compilation and cross-compilation through the
# CROSS_COMPILE prefix.
#
# Usage:
#   make                  Build the writer executable
#   make clean            Remove generated files
#   make CROSS_COMPILE=<toolchain-prefix>
#
# Author: Rajkumar Saravanakumar
#------------------------------------------------------------------------------

# Set default values for CC (native compiler)
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Target to build the "writer" application
TARGET = writer

# Object files
OBJECTS = source/writer.o

# Source files
SOURCES = source/writer.c

# Cross-compilation setup
ifeq ($(CROSS_COMPILE),)
	# If no cross-compiler specified, use the native compiler
	CC = gcc
else
	# Cross-compilation setup: use the specified cross-compiler prefix
	CC = $(CROSS_COMPILE)gcc
endif


#_______Application________
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Rule to compile the C source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Clean target to remove compiled app
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean 

