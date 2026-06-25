/**
 * @file main.c
 * @brief Entry point for the aesdsocket application.
 *
 * This file initializes the system logger, configures signal handling,
 * starts the socket server, and performs resource cleanup before program
 * termination. It serves as the top-level control flow for the application.
 *
 * @author Rajkumar Saravanakumar
 */

#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>

#include "server.h"
#include "signal_handler.h"
#include "debug.h"

int main(int argc, char* argv[]) {
    const char* filepath = "/var/tmp/aesdsocketdata";
    // Attempt to delete the file
    if (remove(filepath) == 0) {
        printf("File %s deleted successfully.\n", filepath);
    }
    else {
        // If the file doesn't exist or cannot be deleted, continue without error
        printf("File %s does not exist or cannot be deleted.\n", filepath);
    }

    int daemon_mode = 0;
    // Check for -d argument
    if (argc > 1 && strcmp(argv[1], "-d") == 0) {
        daemon_mode = 1;
    }

    // Open syslog for logging messages
    openlog("aesdsocket", LOG_PID, LOG_USER);

    setup_signal_handlers();

    // Initialize the Socket Server
    if (server_init(daemon_mode) != 0) {
        syslog(LOG_ERR, "Server initialization failed");
        closelog();
        return EXIT_FAILURE;
    }

    // Run the server, actively checking client connections
    server_run();

    // Server clean up before program exit
    server_cleanup();

    // Close syslog
    closelog();

    debug_log("aesdsocket exiting.");

    return EXIT_SUCCESS;
}