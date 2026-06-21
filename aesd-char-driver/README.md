# AESD Character Device Driver

## Author

Implementated by **Rajkumar Saravanakumar**.

---

## Overview

This directory contains components to build a custom circular buffer character device driver, `aesdchar`.
Implemented using C, Bash scripting, and GNU Make.

The driver provides a character device named:

```text
/dev/aesdchar
```

Data written to the device is stored in a circular buffer containing the most recent write operations. 
Reads return data from the accumulated buffer contents, while custom IOCTL support enables seeking to arbitrary buffer indices and offsets.

The size of the circular buffer (N entries) is configurable and defined in `include/aesd-circular-buffer.h`.

The driver supports concurrent access by multiple threads and processes, and protects shared data using mutex synchronization.

---

## Components

- `source/aesdchar.c` – Core character device driver implementation including file operations (read, write, llseek, ioctl) and module setup/cleanup.
- `source/aesd-circular-buffer.c` – Circular buffer implementation used to store and manage the most recent write entries.

- `include/aesdchar.h` – Driver data structures, debug macros, and function declarations.
- `include/aesd-circular-buffer.h` – Interface for circular buffer utilities and buffer management APIs.
- `include/aesd_ioctl.h` – IOCTL definitions and `aesd_seekto` structure used for user-kernel communication.

- `Makefile` – Kernel module build configuration supporting native and cross-compilation builds.

- `scripts/aesdchar_load` – Loads the kernel module and creates `/dev/aesdchar`.
- `scripts/aesdchar_unload` – Unloads the kernel module and removes the device node.
- `scripts/aesdchar_reload` – Convenience script to reload the driver.
- `scripts/aesdchar-start-stop` – SysV init script for starting, stopping, and restarting the driver at boot.

---

## Features

* Dynamically allocated character device
* Circular buffer storage
* Retains the most recent N completed write operations
* Supports partial writes across multiple `write()` calls
* Read operations based on file position
* Support for `llseek()`
* Custom IOCTL interface
* Thread-safe operation using mutexes
* User-space helper scripts for loading and unloading the module
* SysV init script support

---

## Building

Build the kernel module:

```sh
make modules
```

Clean generated files:

```sh
make clean
```

---

## Usage

### Example

Load the driver:

```sh
./aesdchar_load
```

Write data:

```sh
echo "Hello World" > /dev/aesdchar
echo "Embedded Linux" > /dev/aesdchar
```

Read accumulated contents:

```sh
cat /dev/aesdchar
```

Output:

```text
Hello World
Embedded Linux
```

Unload the driver:

```sh
./aesdchar_unload
```

---

## Testing

Automated testing was performed both locally and through a GitHub Actions-based Continuous Integration (CI) pipeline using a Linux self-hosted runner.

Testing was performed on:

- Native host platform
- ARM64 QEMU virtual machine

Automated validation was carried out using the `full-test.sh` and `unit-test.sh` scripts with the appropriate assignment configuration from `conf` files.

### Driver Functional Testing

The AESD character device driver was validated directly using `drivertest.sh`, which verifies:

- Module load and unload behavior
- Device node creation (`/dev/aesdchar`)
- Multiple sequential write operations to the circular buffer
- Partial read behavior using byte offsets
- Seek functionality using `dd` (`skip`-based access simulation)
- Circular buffer wrap-around correctness
- Data integrity across multiple writes

The test specifically verifies:

- Correct handling of partial reads (starting mid-entry)
- Proper concatenation of stored buffer entries
- Correct retrieval of later buffer entries via seek operations

### Integration Testing with Socket Server

The driver was further validated indirectly through the AESD socket server using `sockettest.sh`.

In this setup, the socket server uses the character device as its backend (when compiled with `USE_AESD_CHAR_DEVICE=1`), allowing end-to-end validation of driver behavior through socket communication.

This test validates:

- Data written via TCP client is stored in the character device
- Incremental accumulation of write operations in the driver buffer
- Correct read-back of driver-stored data through the server
- IOCTL command forwarding (`AESDCHAR_IOCSEEKTO`) through the socket interface
- Seek-based retrieval of buffered driver data via server interaction


All tests completed successfully across both native and ARM64 QEMU environments, with additional automated verification through the CI pipeline.

---

## Architecture

### Core Components

The driver is built around the following core elements:

- Character device (`cdev`)
- Fixed-size circular buffer
- Synchronization using a mutex
- Support for partial and completed write handling

---

### Circular Buffer Design

The driver maintains a fixed-size circular buffer that stores the most recent `N` completed write operations.

When the buffer is full, the oldest entry is overwritten.

Each buffer entry represents a complete line terminated by a newline character (`'\n'`).

---

### Write Semantics

Write operations are considered complete only when a newline character is received.

- Partial writes are accumulated in the current buffer entry
- A newline finalizes the entry and commits it to the circular buffer

Example:

```text
write("Hello")
write(" World")
write("\n")
```

Stored as:

```text
Hello World\n
```

---

### Read Semantics

Reads behave as a continuous stream across all circular buffer entries.

Data is returned based on:

- Current buffer index
- Offset within entry
- File position (`f_pos`)

---

### Seek Support

The driver implements `llseek()` supporting:

- `SEEK_SET`
- `SEEK_CUR`
- `SEEK_END`

File position is interpreted across:

- Buffer index
- Entry offset

Invalid seek positions return an error.

---

### IOCTL Interface

Supports `AESDCHAR_IOCSEEKTO` for direct positioning within the buffer:

```c
struct aesd_seekto {
    uint32_t write_cmd;
    uint32_t write_cmd_offset;
};
```

- `write_cmd` → buffer entry index
- `write_cmd_offset` → byte offset within entry

This updates the file position for subsequent reads.

---

### Synchronization

All shared driver state is protected using a mutex, including:

- Circular buffer access
- Partial write state
- File position updates

---

### Module Lifecycle

#### Initialization

- Allocates device number dynamically
- Initializes circular buffer
- Initializes synchronization primitives
- Registers character device

#### Cleanup

- Removes character device
- Frees allocated resources
- Destroys synchronization primitives
