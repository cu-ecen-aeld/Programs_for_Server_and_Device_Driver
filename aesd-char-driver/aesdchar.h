/**
 * @file aesdchar.h
 * @brief Definitions and interfaces for the AESD character device driver.
 *
 * Declares the device structure, debugging facilities, and function
 * interfaces used by the AESD character driver implementation.
 * 
 * @author Rajkumar Saravanakumar
 */

#ifndef AESD_CHAR_DRIVER_AESDCHAR_H_
#define AESD_CHAR_DRIVER_AESDCHAR_H_

#include <linux/cdev.h>
#include <linux/mutex.h>
#include "aesd-circular-buffer.h"

#define AESD_DEBUG 1  // Remove comment on this line to enable debug

#undef PDEBUG /* undef it, just in case */
#ifdef AESD_DEBUG
#ifdef __KERNEL__
/* This one if debugging is on, and kernel space */
#define PDEBUG(fmt, args...) printk(KERN_DEBUG "aesdchar: " fmt, ##args)
#else
/* This one for user space */
#define PDEBUG(fmt, args...) fprintf(stderr, fmt, ##args)
#endif
#else
#define PDEBUG(fmt, args...) /* not debugging: nothing */
#endif

struct aesd_dev {
    struct cdev cdev;                            /* Char device structure */
    struct aesd_circular_buffer circular_buffer; /* Circular buffer for storing write operations */
    struct mutex lock;                           /* Mutex for thread safety */
    char* partial_write;                         /* Buffer for incomplete write operations */
    size_t partial_write_size;                   /* Size of the partial write */
};

/**
 * @brief Open the AESD character device.
 *
 * Associates the device structure with the file instance for use
 * by subsequent file operations.
 *
 * @param inode Pointer to inode structure.
 * @param file Pointer to file structure.
 *
 * @return 0 on success.
 */
int aesd_open(struct inode*, struct file*);

/**
 * @brief Release the AESD character device.
 *
 * Performs cleanup associated with an open file instance.
 *
 * @param inode Pointer to inode structure.
 * @param file Pointer to file structure.
 *
 * @return 0 on success.
 */
int aesd_release(struct inode*, struct file*);

/**
 * @brief Read data from the character device.
 *
 * Copies data from the circular buffer into user space and advances
 * the file position accordingly.
 *
 * @param file Pointer to file structure.
 * @param buf User buffer.
 * @param count Maximum number of bytes to read.
 * @param f_pos Current file position.
 *
 * @return Number of bytes read on success, negative error code on failure.
 */
ssize_t aesd_read(struct file*, char __user*, size_t, loff_t*);

/**
 * @brief Write data to the character device.
 *
 * Stores completed write operations in the circular buffer. Partial
 * writes are accumulated until a newline character is received.
 *
 * @param file Pointer to file structure.
 * @param buf User buffer.
 * @param count Number of bytes to write.
 * @param f_pos Current file position.
 *
 * @return Number of bytes written on success, negative error code on failure.
 */
ssize_t aesd_write(struct file*, const char __user*, size_t, loff_t*);

/**
 * @brief Initialize and register the AESD character driver.
 *
 * Allocates device resources and prepares the driver for operation.
 *
 * @return 0 on success, negative error code on failure.
 */
int aesd_init_module(void);

/**
 * @brief Unregister the AESD character driver and release resources.
 */
void aesd_cleanup_module(void);

#endif /* AESD_CHAR_DRIVER_AESDCHAR_H_ */
