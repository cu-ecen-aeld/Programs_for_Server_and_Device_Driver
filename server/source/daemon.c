/**
 * @file daemon.c
 * @brief Daemonization support implementation for the AESD socket server.
 *
 * Provides functionality to detach the server process from the controlling
 * terminal and execute it as a background daemon.
 *
 * @author Rajkumar Saravanakumar
 */

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include "daemon.h"

void daemonize() {
    pid_t pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "Failed to fork process");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Parent exits
    }

    // Create a new session and detach from the terminal
    if (setsid() < 0) {
        syslog(LOG_ERR, "Failed to create new session");
        exit(EXIT_FAILURE);
    }

    // Fork again to ensure daemon is not a session leader
    pid = fork();
    if (pid < 0) {
        syslog(LOG_ERR, "Second fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);  // Parent exits again
    }

    // Change working directory to root
    chdir("/");

    // Redirect standard file descriptors to /dev/null
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);

    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w");
    stderr = fopen("/dev/null", "w");
}
