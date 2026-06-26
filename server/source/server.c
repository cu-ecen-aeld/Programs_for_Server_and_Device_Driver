/**
 * @file server.c
 * @brief TCP server initialization, execution, and cleanup routines.
 *
 * This file implements the lifecycle of the aesdsocket server, including
 * socket creation, binding, listening for incoming connections, client
 * acceptance, thread creation for client processing, and resource cleanup
 * during shutdown. It also supports daemon mode operation and coordinates
 * timestamp generation when using file-based storage.
 *
 * @author Rajkumar Saravanakumar
 */

#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <syslog.h>
#include <unistd.h>

#include "client_handler.h"
#include "daemon.h"
#include "debug.h"
#include "globals.h"
#include "server.h"
#include "timestamp.h"

#define PORT "9000"
#define BACKLOG 10

int server_init(int daemon_mode) {
    struct addrinfo hints;
    struct addrinfo* res;

    // Configure server address
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        syslog(LOG_ERR, "getaddrinfo failed");
        return -1;
    }

    // Create server socket
    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        syslog(LOG_ERR, "Socket creation failed");
        freeaddrinfo(res);
        return -1;
    }

    // Bind the socket to the specified port
    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        syslog(LOG_ERR, "Bind failed");
        close(server_fd);
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);

    if (daemon_mode) {
        daemonize();
    }

    pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

    // Initialize mutex before starting any thread
    pthread_mutex_init(&file_mutex, NULL);

    // Start listening for incoming client connections
    if (listen(server_fd, BACKLOG) == -1) {
        syslog(LOG_ERR, "Listen failed");
        close(server_fd);
        return -1;
    }

    return 0;
}

void server_run(void) {
    socklen_t client_addr_len;
    struct sockaddr_storage client_addr;
    thread_node_t* node;

    while (!shutdown_flag) {
        client_addr_len = sizeof(client_addr);

        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);

        if (client_fd == -1) {
            if (shutdown_flag)
                break;
            syslog(LOG_ERR, "Accept failed");
            continue;
        }

        debug_log("Client connected");

#if !USE_AESD_CHAR_DEVICE
        // Create and start the timestamp thread
        pthread_create(&timestamp_thread, NULL, append_timestamp, NULL);
#endif

        node = malloc(sizeof(thread_node_t));
        if (!node) {
            syslog(LOG_ERR, "Memory allocation for cleint node failed");
            close(client_fd);
            continue;
        }

        node->client_fd = client_fd;

        SLIST_INSERT_HEAD(&head, node, entries);  // store the client node thread in the list

        pthread_create(&node->thread_id, NULL, handle_client, node);  // a thread for this client
    }
}

void server_cleanup(void) {
    thread_node_t* node;

    close(server_fd);  // Close server socket

#if !USE_AESD_CHAR_DEVICE
    // Wait for the timestamp thread to finish execution before exiting
    pthread_join(timestamp_thread, NULL);
#endif
    // Join and free all active client threads before exiting to ensure graceful shutdown
    while (!SLIST_EMPTY(&head)) {
        node = SLIST_FIRST(&head);
        pthread_join(node->thread_id, NULL);
        SLIST_REMOVE_HEAD(&head, entries);
        free(node);
    }

#if !USE_AESD_CHAR_DEVICE
    unlink(FILE_PATH);  // Remove the temporary file upon exit
#endif

    pthread_mutex_destroy(&file_mutex);  // Destroy the mutex

    return;
}