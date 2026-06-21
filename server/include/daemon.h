/**
 * @file daemon.h
 * @brief Daemonization support interface for the AESD socket server.
 *
 * Declares functions used to convert the server process into a background
 * daemon.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef DAEMON_H
#define DAEMON_H

/**
 * @brief Converts the current process into a background daemon.
 *
 * Performs the standard daemonization sequence by creating a new session,
 * detaching from the controlling terminal, changing the working directory
 * to the root directory, and redirecting standard input, output, and error
 * streams to /dev/null.
 *
 * On failure, an error is logged and the process terminates.
 */
void daemonize(void);

#endif /* DAEMON_H */
