# Multithreaded TCP Socket Server

## Author

Developed by **Rajkumar Saravanakumar**.

---

## Overview

This directory contains components to build a custom multithreaded TCP socket server, `aesdsocket`. 
Implemented using C, Bash scripting, and GNU Make.

The server listens on port **9000** and accepts connections from multiple clients concurrently. 
Data received from clients is stored either in a regular file or in the AESD character device (A circular buffer device) via a custom driver implementation. 
After receiving a complete packet terminated by a newline character, the server stores the packet and returns the complete contents of the backing store to the client.

The server supports concurrent client connections, two shared storage backends (regular file or AESD character device), and robust operation through daemon mode, synchronization, and graceful shutdown mechanisms.

---

## Components

- `source/main.c`           – Entry point of the application, initializes server and controls lifecycle.
- `source/server.c`         – Core TCP server implementation, handles socket setup, binding, and connection loop.
- `source/client_handler.c` – Per-client thread handler for receiving, processing, and responding to client data.
- `source/timestamp.c`      – Periodic timestamp generation thread implementation (file backend only).
- `source/signal_handler.c` – Handles SIGINT and SIGTERM for graceful shutdown.
- `source/daemon.c`         – Implements daemonization logic for background execution.
- `source/debug.c`          – Simple logging utility for debug traces.
- `source/globals.c`        – Shared global variables (mutex, flags, configuration state).

- `include/` – Header files defining interfaces and shared structures.

- `Makefile` – Build system supporting native and cross-compilation builds.

- `scripts/aesd-start-stop.sh` – Script to start, stop, restart, and check status of the aesdsocket server process in daemon mode.

---

## Features

- TCP socket communication over port `9000`
- Concurrent client handling using a thread-per-connection architecture
- Mutex-based synchronization for thread-safe access to shared resources
- Support for regular file and AESD character device storage backends
- Support for the `AESDCHAR_IOCSEEKTO` ioctl command
- Daemon mode operation
- Periodic timestamp generation
- Signal handling and graceful shutdown
- Syslog-based error and event logging

---

## Building

### Native Build

Build the application:

```sh
make
```

Clean build artifacts:

```sh
make clean
```

### ARM64 Cross Compilation

Build using an ARM64 cross-compilation toolchain:

```sh
make CROSS_COMPILE=aarch64-none-linux-gnu-
```

---

## Usage

### Non-Daemon Mode

Start the server:

```sh
./aesdsocket
```

The server listens on port `9000`.

### Daemon Mode

Start the server as a daemon:

```sh
./aesdsocket -d
```

When daemonized, the process detaches from the terminal and continues execution in the background while listening on port `9000`.

---

## Testing

Automated testing was performed both locally and through a GitHub Actions-based Continuous Integration (CI) pipeline using a Linux self-hosted runner.

Testing was performed on:

- Native host platform
- ARM64 QEMU virtual machine

Automated validation was carried out using the `full-test.sh` and `unit-test.sh` scripts with the appropriate assignment configuration from `conf` files.

The test suite performs end-to-end validation of socket behavior using netcat-based clients, including:

- Server startup and shutdown
- TCP client connection handling on port `9000`
- Incremental data accumulation across multiple client writes
- Correct concatenation and retrieval of stored data
- IOCTL command handling via socket interface (`AESDCHAR_IOCSEEKTO:X,Y`)
- Seek-based read behavior verification
- Backend storage integration (file and AESD character device)
- Timestamp handling in file backend mode
- Signal handling and graceful shutdown

---

## Operation

### Communication Protocol

Clients send newline-terminated packets, for example:

```text
Hello World\n
```

After receiving a complete packet, the server:

1. Stores the received data.
2. Reads the entire backing store.
3. Returns the contents to the client.

### Storage Backends

The server supports two storage backends.

#### Regular File Backend

Data is stored in:

```text
/var/tmp/aesdsocketdata
```

The file is removed during shutdown.

#### AESD Character Device Backend

When compiled with:

```text
USE_AESD_CHAR_DEVICE=1
```

data is stored in:

```text
/dev/aesdchar
```

### IOCTL Support

When using the AESD character device backend, clients may issue commands of the form:

```text
AESDCHAR_IOCSEEKTO:X,Y
```

where:

- `X` is the write command number.
- `Y` is the byte offset within that command.

The server performs the corresponding `AESDCHAR_IOCSEEKTO` operation and returns data beginning from the specified position.

### Threading Model

Each client connection is handled by a dedicated thread responsible for:

- Receiving client data
- Processing IOCTL requests
- Writing data to the backing store
- Returning stored data to the client

When using the regular file backend, a separate timestamp thread periodically appends timestamp records.

### Synchronization

Access to shared storage is protected by a mutex to prevent race conditions between client threads and the timestamp thread.

### Timestamp Generation

When using the regular file backend, timestamp records are appended approximately every 10 seconds in the format:

```text
timestamp: Wed, 19 Jun 2026 13:47:52 +0000
```

Timestamp generation is disabled when using the AESD character device backend.

### Signal Handling

The server handles:

- `SIGINT`
- `SIGTERM`

Upon receiving either signal, the server:

1. Stops accepting new connections.
2. Terminates worker threads gracefully.
3. Cleans up resources.
4. Removes temporary files when applicable.
5. Exits cleanly.

### Logging

The server uses `syslog()` for event and error reporting, including:

- Client connections
- Errors
- Signal reception
- Device and file access failures
