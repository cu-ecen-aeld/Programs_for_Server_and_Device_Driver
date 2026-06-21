/**
 * @file signal_handler.h
 * @brief Signal handling interface for the AESD socket server.
 *
 * Declares functions used to configure signal handlers and manage
 * graceful server termination.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

/**
 * @brief Handles process termination signals.
 *
 * Logs the received signal and sets the shutdown flag to notify
 * server threads to terminate gracefully.
 *
 * @param signum Signal number received by the process.
 */
void cleanup_and_exit(int signum);

/**
 * @brief Registers signal handlers for graceful shutdown.
 *
 * Configures handlers for SIGINT and SIGTERM so that the server
 * can perform orderly cleanup before exiting.
 */
void setup_signal_handlers(void);

#endif /* SIGNAL_HANDLER_H */
