/**
 * @file globals.c
 * @brief Global data definitions for the AESD socket server.
 *
 * Defines shared variables used across multiple modules, including
 * synchronization primitives, thread management data, and runtime
 * configuration parameters.
 *
 * @author Rajkumar Saravanakumar
 */

#include <pthread.h>
#include <signal.h>

#include "globals.h"

int server_fd = -1;

pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

volatile sig_atomic_t shutdown_flag = 0;

pthread_t timestamp_thread;

struct thread_list head =
    SLIST_HEAD_INITIALIZER(head);

#if USE_AESD_CHAR_DEVICE
const char* FILE_PATH = "/dev/aesdchar";
#else
const char* FILE_PATH = "/var/tmp/aesdsocketdata";
#endif