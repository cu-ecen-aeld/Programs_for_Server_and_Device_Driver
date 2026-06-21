/**
 * @file client_handler.h
 * @brief Client connection handling interface for the AESD socket server.
 *
 * Declares functions responsible for servicing client connections and
 * processing socket communication requests.
 *
 * @author Rajkumar Saravanakumar
 */

#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

/**
 * @brief Handles communication with a connected client.
 *
 * Receives data from the client until a newline character is encountered,
 * processes AESD ioctl commands when applicable, appends normal data to
 * the configured storage device or file, and sends the current contents
 * back to the client. The function runs as a dedicated thread for each
 * client connection.
 *
 * @param arg Pointer to the thread node containing client connection
 *            information.
 *
 * @return NULL. The thread terminates using pthread_exit().
 */
void* handle_client(void* arg);

#endif /* CLIENT_HANDLER_H */
