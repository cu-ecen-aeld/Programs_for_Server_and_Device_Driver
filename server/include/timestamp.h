/**
 * @file timestamp.h
 * @brief Timestamp thread interface for the AESD socket server.
 *
 * Declares functions used to periodically generate and append
 * timestamp records to the server data store.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef TIMESTAMP_H
#define TIMESTAMP_H

/**
 * @brief Periodically appends timestamps to the server data file.
 *
 * Runs as a dedicated thread and writes a formatted timestamp entry
 * every 10 seconds while the server is active. File access is
 * synchronized using a mutex to prevent concurrent modifications.
 *
 * @param arg Unused thread argument.
 *
 * @return NULL upon thread termination.
 */
void* append_timestamp(void* arg);

#endif /* TIMESTAMP_H */
