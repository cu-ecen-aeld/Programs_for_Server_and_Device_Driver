# Makefile for aesdsocket program

USE_AESD_CHAR_DEVICE ?= 1


# Target executable name
TARGET = aesdsocket

# Source files and Object files
SRCS = aesdsocket.c
OBJS = $(SRCS:%.c=%.o)




ifeq ($(HOST),1)
    CC = gcc
    SYSROOT =
    CFLAGS += -Wall -Wextra -O2 -g -pthread -lrt
    CFLAGS += -DUSE_AESD_CHAR_DEVICE=$(USE_AESD_CHAR_DEVICE)
    LDFLAGS :=
else
    CC = aarch64-poky-linux-gcc
    SYSROOT ?= /
    CFLAGS ?= $(TARGET_CC_ARCH)
    CFLAGS += --sysroot=$(SYSROOT) -pthread -lrt
    CFLAGS += -DUSE_AESD_CHAR_DEVICE=$(USE_AESD_CHAR_DEVICE)
    LDFLAGS ?= $(TARGET_LDFLAGS)
    LDFLAGS += --sysroot=$(SYSROOT) -lgcc -lc
endif


#LD ?= aarch64-poky-linux-ld  
CFLAGS = -Wall -Wextra -g
#LDFLAGS += $(TARGET_LDFLAGS)
 

# Cross-compilation setup
#ifeq ($(CROSS_COMPILE),)
	# If no cross-compiler specified, use the native compiler
#	CC = gcc
#	LD = ld
#else
	# Cross-compilation setup: use the specified cross-compiler prefix
#	CC = $(CROSS_COMPILE)gcc
#	LD = $(CROSS_COMPILE)ld
#endif

# Output directory
#OUT_DIR := ./server

# Default target
default: all

# Build target
all: $(TARGET)

# Compile the target binary
$(TARGET): $(OBJS)

	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .c files into .o files in OUT_DIR
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) *.o
	#rm -rf $(OUT_DIR)

# PHONY targets (not actual files)
.PHONY: all clean default

