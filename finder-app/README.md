# Finder Application

## Author

Implementation by **Rajkumar Saravanakumar**.

---

## Overview

The Finder Application is a collection of utilities providing functionality for creating files 
containing specified text and searching directories for files and matching lines.

The project consists of Bash shell scripts and a C implementation of the writer utility, built using GNU Make.

---

## Components

- `finder.sh` – Searches a directory tree and reports number of files and matching lines for a given string.
- `finder-test.sh` – Test script used to verify finder and writer functionality.
- `writer.sh` – Creates required directories and writes a string to a file.

- `writer.c` – Native C implementation of the writer utility.

- `Makefile` – Build instructions for the C writer application.                                                    |

---

## Features

* Recursive file searching using `finder.sh`
* File creation and string writing utilities
* Native C and shell script implementations
* POSIX file I/O operations
* Automatic creation of missing parent directories
* Syslog-based error and debug logging
* Cross-compilation support
* Automated testing and validation scripts

---

## Building

Build the writer application:

```sh
make
```

Clean build artifacts:

```sh
make clean
```

### Cross Compilation

The Makefile supports cross compilation through the `CROSS_COMPILE` variable.

Example:

```sh
make CROSS_COMPILE=aarch64-none-linux-gnu-
```

---

## Usage

### Finder Script

Searches a directory tree recursively and reports the number of files and matching lines containing a specified string.

```sh
./finder.sh <filesdir> <searchstr>
```

#### Arguments

* **filesdir** – Directory to search recursively.
* **searchstr** – String to search for.

#### Example

```sh
./finder.sh /tmp/example "hello"
```

Example output:

```text
The number of files are 5 and the number of matching lines are 3
```

### Writer Script

Creates a file and writes a string to it, creating any missing parent directories as required.

```sh
./writer.sh <writefile> <writestr>
```

#### Arguments

* **writefile** – Path of the file to create.
* **writestr** – String to write to the file.

#### Example

```sh
./writer.sh /tmp/a/b/output.txt "Embedded Linux"
```

### Writer Application

Native C implementation of the writer utility.

```sh
./writer <writefile> <writestr>
```

The application performs the same function as `writer.sh`.

#### Arguments

* **writefile** – Path of the file to create.
* **writestr** – String to write to the file.

#### Example

```sh
./writer /tmp/example.txt "Hello World"
```

---

## Testing

Automated testing was performed both locally and through a GitHub Actions-based Continuous Integration (CI) pipeline using a Linux self-hosted runner.

Validation was carried out using the `full-test.sh` and `unit-test.sh` scripts with the appropriate assignment configuration from `conf` files.

The `finder-test.sh` script automates functional validation of the finder and writer utilities.

It:

1. Creates a temporary test directory.
2. Generates multiple files using the C writer application (`writer`).
3. Verifies correct file and directory creation.
4. Executes `finder.sh` to search for the generated string.
5. Confirms that the reported number of files and matching lines are correct.

The `writer.sh` script provides equivalent functionality and was used for manual verification.

Run the test script:

```sh
./finder-test.sh
```

In addition to manual testing, the project was successfully validated using a GitHub Actions-based 
Continuous Integration (CI) pipeline running on a Linux self-hosted runner.
