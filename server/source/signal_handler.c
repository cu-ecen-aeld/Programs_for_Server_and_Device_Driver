/**
 * @file signal_handler.c
 * @brief Signal handling implementation for the AESD socket server.
 *
 * Provides functionality for handling termination signals and initiating
 * graceful server shutdown.
 *
 * @author Rajkumar Saravanakumar
 */

#include <signal.h>
#include <syslog.h>

#include "globals.h"
#include "signal_handler.h"

void cleanup_and_exit(int signum) {
    syslog(LOG_INFO, "Caught signal %d, exiting", signum);

    shutdown_flag = 1;  // Flag to terminate client processing
}

void setup_signal_handlers(void) {
    struct sigaction sa;

    sa.sa_handler = cleanup_and_exit;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}