/**
 * @file debug.h
 * @brief Debug logging interface for the AESD socket server.
 *
 * Declares utility functions used for recording debug information during
 * server execution.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef DEBUG_H
#define DEBUG_H

/**
 * @brief Writes a debug message to the debug log file.
 *
 * Appends the specified message to the debug log and flushes the output
 * immediately to ensure it is written to persistent storage.
 *
 * @param message Null-terminated string containing the message to log.
 */
void debug_log(const char* message);

#endif /* DEBUG_H */
