/**
 * @file threading.c
 * @brief Thread creation and mutex synchronization utilities.
 *
 * Provides helper functions for creating threads that acquire and
 * release a mutex after configurable delays. These routines are
 * intended to demonstrate thread synchronization using POSIX threads.
 *
 * @author Rajkumar Saravanakumar
 */

#include "threading.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg, ...)
// #define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg, ...) printf("threading ERROR: " msg "\n", ##__VA_ARGS__)

/**
 * @brief Thread entry function for mutex synchronization.
 *
 * Waits for a specified interval before acquiring the mutex, holds
 * the mutex for a configurable duration, and then releases it.
 * The completion status is recorded in the associated thread_data
 * structure.
 *
 * @param thread_param
 * Pointer to a thread_data structure containing synchronization
 * parameters and status information.
 *
 * @return
 * Pointer to the supplied thread_data structure.
 */
void* threadfunc(void* thread_param) {
    // struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data* thread_func_args = (struct thread_data*)thread_param;

    // Wait before trying to obtain the mutex
    usleep(thread_func_args->wait_to_obtain_ms * 1000);

    // Try to lock the mutex
    if (pthread_mutex_lock(thread_func_args->mutex) != 0) {
        thread_func_args->thread_complete_success = false;
        pthread_exit(thread_func_args);
    }

    // Hold the mutex for the specified time
    usleep(thread_func_args->wait_to_release_ms * 1000);

    // Unlock the mutex
    pthread_mutex_unlock(thread_func_args->mutex);

    // Mark the thread as successful
    thread_func_args->thread_complete_success = true;

    // Return the thread data structure
    pthread_exit(thread_func_args);

    return thread_param;
}

/**
 * @brief Create a thread that performs timed mutex synchronization.
 *
 * Allocates and initializes a thread_data structure and starts a
 * thread executing threadfunc().
 *
 * @param thread
 * Pointer to the thread identifier returned by pthread_create().
 *
 * @param mutex
 * Pointer to the mutex used by the created thread.
 *
 * @param wait_to_obtain_ms
 * Delay, in milliseconds, before attempting to acquire the mutex.
 *
 * @param wait_to_release_ms
 * Duration, in milliseconds, to hold the mutex before releasing it.
 *
 * @retval true
 * Thread creation completed successfully.
 *
 * @retval false
 * Memory allocation or thread creation failed.
 */
bool start_thread_obtaining_mutex(pthread_t* thread, pthread_mutex_t* mutex, int wait_to_obtain_ms, int wait_to_release_ms) {
    // Allocate memory for thread_data
    struct thread_data* data = malloc(sizeof(struct thread_data));
    if (data == NULL) {
        return false;  // Memory allocation failed
    }

    // Initialize thread_data fields
    data->mutex = mutex;
    data->wait_to_obtain_ms = wait_to_obtain_ms;
    data->wait_to_release_ms = wait_to_release_ms;
    data->thread_complete_success = false;

    // Create the thread
    if (pthread_create(thread, NULL, threadfunc, data) != 0) {
        free(data);  // Free allocated memory if thread creation fails
        return false;
    }

    return true;  // Thread successfully started
}
