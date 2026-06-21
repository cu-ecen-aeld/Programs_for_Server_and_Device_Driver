/**
 * @file server.h
 * @brief Function declarations for TCP server management.
 *
 * This header provides interfaces for initializing, running, and shutting
 * down the aesdsocket server. These functions manage the server lifecycle
 * and coordinate the creation and cleanup of resources associated with
 * client connections.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef SERVER_H
#define SERVER_H

/**
 * @brief Initialize the TCP server.
 *
 * Creates and configures the listening socket and optionally
 * daemonizes the process.
 *
 * @param daemon_mode Nonzero to run as a daemon, zero to run in
 *                    the foreground.
 *
 * @return 0 on success, -1 on failure.
 */
int server_init(int daemon_mode);

/**
 * @brief Run the main server loop.
 *
 * Accepts incoming client connections and creates a thread to
 * handle each client until shutdown is requested.
 */
void server_run(void);

/**
 * @brief Perform server shutdown and resource cleanup.
 *
 * Closes the listening socket and releases resources associated
 * with active threads and data storage.
 */
void server_cleanup(void);

#endif /* SERVER_H */
