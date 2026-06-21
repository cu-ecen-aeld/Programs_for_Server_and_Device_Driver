# Manual Embedded Linux Build System

## Author

Implementation by **Rajkumar Saravanakumar**.

---

## Overview

This directory contains a fully automated shell-based system for building a minimal ARM64 Linux environment from source. 
Implemented using Bash scripting.

The build pipeline is designed for experimentation and validation of embedded Linux concepts before integrating the device driver and applications into a Yocto and Buildroot-based embedded linux systems.

It produces an ARM64 Linux kernel image and a bootable initramfs for execution under QEMU, containing a BusyBox-based userspace and user-defined applications.

---

## Features

- Automated end-to-end ARM64 Embedded Linux system construction
- Linux kernel compilation from source
- BusyBox-based root filesystem creation
- Minimal userspace and filesystem hierarchy setup
- Cross-compilation support using ARM64 toolchains
- Automatic library dependency integration
- Device node creation
- Integration of custom user-space applications
- Initramfs generation for execution under QEMU
- Support for experimentation and validation prior to Yocto and Buildroot integration

--- 

## Usage

```bash
./manual-linux.sh [output_directory]
```

---

## Output

After execution, the following artifacts are generated in the output directory:

- `Image` → Linux kernel image
- `initramfs.cpio.gz` → Root filesystem image
- `rootfs/` → Temporary staging filesystem

---

## Testing

Automated testing was performed both locally and through a GitHub Actions-based Continuous Integration (CI) pipeline using a Linux self-hosted runner.

The system was validated through an automated end-to-end testing framework that uses the implementation 
to build the embedded Linux system image along with user-space applications and executes under QEMU.

Automated validation was carried out using the `full-test.sh` and `unit-test.sh` scripts with the appropriate assignment configuration from `conf` files.

Testing is performed in two stages:

The validation includes:

- Building the full system using `manual-linux.sh`, including kernel compilation, BusyBox root filesystem creation, user-space integration, and initramfs generation
- Booting the generated image in a QEMU ARM64 virtual machine
- Verifying successful system execution, including application startup and proper system shutdown behavior

The test framework runs both the build and runtime validation automatically, monitors serial output, and confirms successful execution using a completion indicator from the system. The process is terminated automatically after success or timeout.

---

### Automated Execution

Testing is orchestrated using an autograder script that:

- Executes the build process (unless skipped)
- Boots the system in QEMU
- Validates execution using log-based completion detection
- Terminates QEMU after successful execution or timeout

All tests are also executed in a GitHub Actions-based CI environment using a Linux self-hosted runner to ensure reproducibility across platforms.
