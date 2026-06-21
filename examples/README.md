# System Programming Examples

## Author

Implementation by **Rajkumar Saravanakumar**.

---

## Overview

This directory contains standalone Linux system programming examples written in C. They demonstrate core POSIX concepts used in embedded and systems development.

These examples cover process control, execution models, threading, and synchronization.

These examples serve as building blocks and practical understanding of:
- Embedded Linux applications
- Multithreaded server design
- Kernel/user-space interaction

---

## Components

- `systemcalls.c`  – Demonstrates command execution using `system()`, `fork()`, `execv()`, and output redirection.
- `systemcalls.h`  – Function declarations and interfaces for executing external commands and redirecting command output.

- `threading.c`  – Demonstrates thread creation, synchronization, and timing control using POSIX threads and mutexes.
- `threading.h`  – Data structures and function declarations for the threading example.

---

## Features

- Process creation using `fork()`
- Program execution using `execv()`
- Shell command execution using `system()`
- Output redirection
- POSIX thread creation and management
- Mutex-based thread synchronization
- Timing control and thread coordination
- Variadic function interfaces
- Child process monitoring using `waitpid()`
- Basic error handling for system calls
