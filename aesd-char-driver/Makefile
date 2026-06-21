#------------------------------------------------------------------------------
# File: Makefile
#
# Description:
#   Builds the AESD character device driver as a loadable kernel module.
#   Supports both native and cross-compilation through the Linux kernel
#   build system.
#
# Usage:
#   make modules               Build the kernel module
#   make clean                 Remove generated build artifacts
#
# Author:
#   Rajkumar Saravanakumar
#------------------------------------------------------------------------------

# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSCULL_DEBUG  
else
  DEBFLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBFLAGS)

ifneq ($(KERNELRELEASE),)  # Call from kernel build system

obj-m	:= aesdchar.o
aesdchar-y := source/aesd-circular-buffer.o source/main.o # .c files to compile and link into aesdchar.ko

ccflags-y += -I$(src)/include

else  # Call from current directory

KERNELDIR ?= $(KERNEL_SRC)  # remove for native compilation
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build  #remove for cross compilation

PWD       := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

endif

clean:
	rm -rf source/*.o *~ core .depend source/.*.cmd *.ko *.mod.c .tmp_versions

