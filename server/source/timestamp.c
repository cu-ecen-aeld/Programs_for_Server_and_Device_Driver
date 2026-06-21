/**
 * @file timestamp.c
 * @brief Timestamp thread implementation for the AESD socket server.
 *
 * Provides functionality for periodically appending timestamp entries
 * to the server data file when operating in file-backed mode.
 *
 * @author Rajkumar Saravanakumar
 */

#include <pthread.h>
#include <stdio.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "globals.h"
#include "timestamp.h"

void* append_timestamp(void* arg) {
    (void)arg;  // Mark argument as unused

    while (!shutdown_flag) {
        // Instead of sleeping 10 sec at once, check shutdown_flag every 1 sec
        for (int i = 0; i < 10 && !shutdown_flag; i++) {
            sleep(1);
        }

        if (shutdown_flag)
            break;  // Exit immediately if shutdown is requested

        time_t now = time(NULL);
        struct tm* tm_info = localtime(&now);
        char time_str[100];

        strftime(time_str, sizeof(time_str), "timestamp: %a, %d %b %Y %H:%M:%S %z\n", tm_info);

        // Lock the file to avoid race conditions
        pthread_mutex_lock(&file_mutex);

        FILE* file = fopen("/var/tmp/aesdsocketdata", "a");
        if (file) {
            fputs(time_str, file);
            fclose(file);
        }
        else {
            syslog(LOG_ERR, "Failed to open file for timestamp writing");
        }

        pthread_mutex_unlock(&file_mutex);
    }
    return NULL;
}