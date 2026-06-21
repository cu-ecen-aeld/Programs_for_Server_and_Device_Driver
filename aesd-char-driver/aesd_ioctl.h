/**
 * @file aesd_ioctl.h
 * @brief Definitions for ioctl commands supported by the AESD character driver.
 *
 * Defines the data structures and ioctl command codes used for
 * communication between user space applications and the AESD
 * character device.
 * 
 * @author Rajkumar Saravanakumar
 */

#ifndef AESD_IOCTL_H
#define AESD_IOCTL_H

#ifdef __KERNEL__
#include <asm-generic/ioctl.h>
#include <linux/types.h>
#else
#include <stdint.h>
#include <sys/ioctl.h>
#endif

/**
 * @brief Parameters used by the AESDCHAR_IOCSEEKTO ioctl command.
 *
 * Describes a seek position as a write command index and an offset
 * within that command.
 */
struct aesd_seekto {
    /**
     * The zero referenced write command to seek into
     */
    uint32_t write_cmd;
    /**
     * The zero referenced offset within the write
     */
    uint32_t write_cmd_offset;
};

// Pick an arbitrary unused value from https://github.com/torvalds/linux/blob/master/Documentation/userspace-api/ioctl/ioctl-number.rst
#define AESD_IOC_MAGIC 0x16

// Define a write command from the user point of view, use command number 1
#define AESDCHAR_IOCSEEKTO _IOWR(AESD_IOC_MAGIC, 1, struct aesd_seekto)

/**
 * @brief Maximum number of ioctl commands supported.
 *
 * Used for ioctl command validation and bounds checking.
 */
#define AESDCHAR_IOC_MAXNR 1

#endif /* AESD_IOCTL_H */
