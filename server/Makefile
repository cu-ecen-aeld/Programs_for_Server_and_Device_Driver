#------------------------------------------------------------------------------
# Makefile
#
# Build configuration for the aesdsocket TCP server application.
#
# Supports native and cross-compilation, with optional integration
# with the AESD character device driver. Provides targets for building
# the executable and cleaning generated artifacts.
#
# Usage:
#   make                           Build the application
#   make clean                     Remove generated files
#   make HOST=1                    Build natively on the host system
#   make USE_AESD_CHAR_DEVICE=0    Use /var/tmp/aesdsocketdata instead of
#                                  the AESD character device
#
# Author: Rajkumar Saravanakumar
#------------------------------------------------------------------------------


#HOST ?= 1
USE_AESD_CHAR_DEVICE ?= 1

# Target executable name
TARGET = aesdsocket

# Source files and Object files
SRCS = source/main.c \
	   source/server.c \
	   source/client_handler.c \
	   source/timestamp.c \
	   source/daemon.c \
	   source/signal_handler.c \
	   source/debug.c \
	   source/globals.c

OBJS = $(SRCS:%.c=%.o)

ifeq ($(HOST),1)
    CC = gcc
    SYSROOT =
    CFLAGS += -Wall -Wextra -O2 -g -pthread -lrt
    #CFLAGS += -DUSE_AESD_CHAR_DEVICE=$(USE_AESD_CHAR_DEVICE)
    CPPFLAGS += -Iinclude -I../aesd-char-driver
    LDFLAGS :=
else
    CC = aarch64-poky-linux-gcc
    SYSROOT ?= /
    CFLAGS ?= $(TARGET_CC_ARCH)
    CFLAGS += --sysroot=$(SYSROOT) -pthread -lrt
    CFLAGS += -DUSE_AESD_CHAR_DEVICE=$(USE_AESD_CHAR_DEVICE)
    CPPFLAGS += -Iinclude -I../aesd-char-driver
    LDFLAGS ?= $(TARGET_LDFLAGS)
    LDFLAGS += --sysroot=$(SYSROOT) -lgcc -lc
endif

# Default target
default: all

# Build target
all: $(TARGET)

# Compile the target binary
$(TARGET): $(OBJS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files into .o files in OUT_DIR
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) $(OBJS)

# PHONY targets (not actual files)
.PHONY: all clean default

