/**
 * @file client_handler.c
 * @brief Client connection handling implementation for the AESD socket server.
 *
 * Receives data from connected clients, processes ioctl commands when
 * using the AESD character device, appends incoming data to the backing
 * storage, and returns stored contents to the client.
 *
 * @author Rajkumar Saravanakumar
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <syslog.h>
#include <unistd.h>

#include "aesd_ioctl.h"
#include "client_handler.h"
#include "globals.h"

void* handle_client(void* arg) {
    debug_log("Changes are reflected in the build");

    thread_node_t* node = (thread_node_t*)arg;
    int client_fd = node->client_fd;
    ssize_t bytes_read;

    char recv_buffer[1024];
    char* full_msg = NULL;
    size_t total_len = 0;

    while (1) {
        // Reset message state for each new complete line
        total_len = 0;
        free(full_msg);
        full_msg = NULL;

        //_____Receive until newline is found______
        while ((bytes_read = recv(client_fd, recv_buffer, sizeof(recv_buffer), 0)) > 0) {
            char* new_buf = realloc(full_msg, total_len + bytes_read);
            if (!new_buf) {
                syslog(LOG_ERR, "Memory allocation failed");
                free(full_msg);
                close(client_fd);
                pthread_exit(NULL);
            }
            full_msg = new_buf;
            memcpy(full_msg + total_len, recv_buffer, bytes_read);
            total_len += bytes_read;

            // Check for newline
            if (memchr(recv_buffer, '\n', bytes_read))
                break;
        }

        if (bytes_read == -1) {
            syslog(LOG_ERR, "Receive failed: %s", strerror(errno));
            break;
        }
        else if (bytes_read == 0) {
            // Client closed connection
            break;
        }

        // --------- HANDLE SPECIAL IOCTL COMMAND ----------
        if (strncmp(full_msg, "AESDCHAR_IOCSEEKTO:", 19) == 0) {
            unsigned int write_cmd = 0, write_cmd_offset = 0;
            if (sscanf(full_msg + 19, "%u,%u", &write_cmd, &write_cmd_offset) == 2) {
                struct aesd_seekto seekto = {
                    .write_cmd = write_cmd,
                    .write_cmd_offset = write_cmd_offset};

                pthread_mutex_lock(&file_mutex);
                int file_fd = open(FILE_PATH, O_RDWR);
                if (file_fd == -1) {
                    syslog(LOG_ERR, "Failed to open device file for ioctl");
                    pthread_mutex_unlock(&file_mutex);
                    break;
                }

                // Perform the ioctl
                if (ioctl(file_fd, AESDCHAR_IOCSEEKTO, &seekto) == -1) {
                    syslog(LOG_ERR, "ioctl failed: %s", strerror(errno));
                    close(file_fd);
                    pthread_mutex_unlock(&file_mutex);
                    continue;  // skip to next message
                }
                memset(recv_buffer, 0, sizeof(recv_buffer));
                // Read from updated position and send back
                while ((bytes_read = read(file_fd, recv_buffer, sizeof(recv_buffer))) > 0) {
                    send(client_fd, recv_buffer, bytes_read, 0);
                }

                lseek(file_fd, 0, SEEK_END);  // Reset position to end for future appends
                close(file_fd);
                free(full_msg);
                full_msg = NULL;
                total_len = 0;
                pthread_mutex_unlock(&file_mutex);

                continue;  // do not fall through to write path
            }
            else {
                syslog(LOG_ERR, "Malformed ioctl command: %s", full_msg);
                continue;
            }
        }

        // --------- Write normal full message to file ----------
        pthread_mutex_lock(&file_mutex);
        int file_fd = open(FILE_PATH, O_RDWR | O_CREAT | O_APPEND, 0666);
        if (file_fd == -1) {
            syslog(LOG_ERR, "Failed to open file for writing");
            pthread_mutex_unlock(&file_mutex);
            break;
        }

        ssize_t written = 0;

        while (written < (ssize_t)total_len) {
            ssize_t ret = write(file_fd, full_msg + written, total_len - written);
            if (ret == -1) {
                syslog(LOG_ERR, "write failed: %s", strerror(errno));
                break;
            }
            written += ret;
        }

        close(file_fd);
        pthread_mutex_unlock(&file_mutex);

        // Send back file contents

        pthread_mutex_lock(&file_mutex);

        file_fd = open(FILE_PATH, O_RDONLY);
        if (file_fd == -1) {
            syslog(LOG_ERR, "Failed to open file for reading");
            pthread_mutex_unlock(&file_mutex);
            break;
        }

        while ((bytes_read = read(file_fd, recv_buffer, sizeof(recv_buffer))) > 0) {
            send(client_fd, recv_buffer, bytes_read, 0);
        }

        close(file_fd);
        pthread_mutex_unlock(&file_mutex);
    }

    free(full_msg);
    close(client_fd);

    SLIST_REMOVE(&head, node, thread_node, entries);
    free(node);

    pthread_exit(NULL);
}