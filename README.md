# Advanced Embedded Linux Development Projects

## Author

**Rajkumar Saravanakumar**

Developed as part of the Advanced Embedded Linux Development specialization from the University of Colorado Boulder.

---

## Overview

This repository contains a collection of Embedded Linux system programming and kernel development projects.

The repository includes implementations of a Linux character device driver, a multithreaded TCP socket server, 
scripts for constructing an Embedded Linux system from scratch, user-space utilities, and example system programs. 
These components were subsequently integrated into separate Yocto- and Buildroot-based Embedded Linux systems and validated under QEMU.

The repository is validated manually and by using a GitHub Actions-based Continuous Integration (CI) pipeline 
that builds and tests components in a Linux-based environment using self-hosted runner.

---

## Core Repository Structure

```text
.
├── aesd-char-driver/       # Linux character device driver (Custom circular buffer)
├── server/                 # Multithreaded TCP socket server
├── finder-app/             # File writer utility and shell scripts
├── manual-embedded-linux/  # Manual Embedded Linux image build script
├── examples/               # Example system programs and threading utilities
├── arm-gnu-toolchain/      # ARM64 cross-compilation runtime libraries
├── assignment-autotest/    # Automated test framework submodule
└── README.md
```

Each component contains its own README with additional details regarding design, usage, and build instructions.

---

## Projects

### AESD Character Device Driver

Linux kernel character device driver providing custom circular buffer storage and custom IOCTL support.

See `aesd-char-driver/README.md` for details.

### AESD Socket Server

Multithreaded TCP socket server supporting both file and character device backends for storage.

See `server/README.md` for details.

### Manual Embedded Linux Build

Scripts for building an ARM64 Linux kernel, constructing a minimal BusyBox-based Embedded Linux system from source, and generating an initramfs image for QEMU.

See `manual-embedded-linux/README.md` for details.

### Finder Application

User-space utilities and shell scripts for recursive file searching and file creation with specific content.

See `finder-app/README.md` for details.

### Example System Programs

Collection of programs demonstrating Linux system programming concepts commonly used in Embedded Linux development.

See `examples/README.md` for details.

---

## Testing

The repository is validated through automated test suites testing functionalities and integrations across multiple components.

Testing is performed in both local and GitHub Actions-based Continuous Integration (CI) pipeline using a Linux self-hosted runner.

Targets:

- Native host platform
- ARM64 QEMU virtual machine

Automated test suites are used to validate individual components as well as full system integration, including:

- Linux character device driver functionality
- Socket server communication and concurrency behavior
- File system and script-based utilities (finder and writer applications)
- End-to-end integration of user-space applications with kernel-space components
- Embedded Linux build and boot workflow using initramfs-based system images

Refer to the project-specific `README.md` files for details regarding validation procedures and functional testing.

---

## Technologies

* C
* Bash Scripting
* GNU Make
* POSIX APIs
* Linux System Programming
* Socket Programming
* Multithreading with Pthreads
* Linux Kernel Development
* Character Device Drivers
* IOCTL Interfaces
* ARM64 Cross-Compilation
* BusyBox
* QEMU
* GitHub Actions

---

## Yocto and Buildroot Integration

The software contained in this repository was subsequently integrated into separate Yocto- and Buildroot-based Embedded Linux systems.

Both integrations automate the retrieval, cross-compilation, and deployment of:

* User-space applications (server and finder app)
* Character device driver

The complete Yocto and Buildroot integrations are documented separately in their respective repositories.
