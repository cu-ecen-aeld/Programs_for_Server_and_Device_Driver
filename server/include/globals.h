/**
 * @file globals.h
 * @brief Global data declarations for the AESD socket server.
 *
 * Declares shared variables and data structures used by multiple
 * modules within the server implementation.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <signal.h>
#include <sys/queue.h>

/**
 * @brief Node used to track an active client thread.
 *
 * Each connected client is serviced by a dedicated thread. A thread node
 * stores the thread identifier and client socket descriptor and is inserted
 * into a singly linked list to facilitate cleanup during server shutdown.
 */
typedef struct thread_node {
    /** POSIX thread identifier for the client thread. */
    pthread_t thread_id;

    /** Socket descriptor associated with the client connection. */
    int client_fd;

    /** Singly linked list entry. */
    SLIST_ENTRY(thread_node)
    entries;

} thread_node_t;

/**
 * @brief Singly linked list type used to maintain active client threads.
 */
SLIST_HEAD(thread_list, thread_node);

/*-------------------------------------------------------------------------
 * Global variables
 *------------------------------------------------------------------------*/

/** Server socket file descriptor. */
extern int server_fd;

/** Mutex used to synchronize access to the backing file or character device. */
extern pthread_mutex_t file_mutex;

/** Flag indicating that a shutdown has been requested. */
extern volatile sig_atomic_t shutdown_flag;

/** Thread responsible for periodically appending timestamps. */
extern pthread_t timestamp_thread;

/** Head of the linked list containing active client thread nodes. */
extern struct thread_list head;

/**
 * Path to the backing storage used by the server.
 *
 * Depending on the build configuration, this may refer to either the
 * aesdchar character device or a regular file.
 */
extern const char* FILE_PATH;

#endif /* GLOBALS_H */