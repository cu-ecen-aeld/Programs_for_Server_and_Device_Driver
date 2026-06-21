/**
 * @file debug.c
 * @brief Debug logging implementation for the AESD socket server.
 *
 * Provides utility functions for writing debug messages to a dedicated
 * log file.
 *
 * @author Rajkumar Saravanakumar
 */

#include <stdio.h>

#include "debug.h"

#define DEBUG_LOG_FILE "/tmp/aesdsocket.log"

void debug_log(const char* message) {
    FILE* file = fopen(DEBUG_LOG_FILE, "a");
    if (file) {
        fprintf(file, "%s\n", message);
        fflush(file);  // Ensure message is written immediately
        fclose(file);
    }
}