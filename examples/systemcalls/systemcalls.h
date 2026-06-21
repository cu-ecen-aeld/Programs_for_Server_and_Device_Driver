/**
 * @file systemcalls.h
 * @brief Interfaces for executing external commands.
 *
 * Provides utility functions for executing commands using system(),
 * fork(), execv(), and waitpid(). Also supports redirecting command
 * output to a file.
 *
 * @author Rajkumar Saravanakumar
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Execute a command using the system() library call.
 *
 * Invokes the specified command using the host shell and waits for
 * completion before returning.
 *
 * @param cmd
 * Null-terminated command string to execute.
 *
 * @retval true
 * The command executed successfully and returned an exit status of zero.
 *
 * @retval false
 * The system() call failed or the command returned a nonzero exit status.
 */
bool do_system(const char* command);

/**
 * @brief Execute a command using fork(), execv(), and waitpid().
 *
 * Creates a child process to execute the specified command and waits
 * for the child to terminate. The command path must be absolute.
 *
 * @param count
 * Number of arguments supplied through the variable argument list.
 *
 * @param ...
 * Command path followed by its arguments. The first argument must be
 * the absolute path of the executable.
 *
 * @retval true
 * The command completed successfully and returned an exit status of zero.
 *
 * @retval false
 * An error occurred or the command returned a nonzero exit status.
 */
bool do_exec(int count, ...);

/**
 * @brief Execute a command and redirect its standard output to a file.
 *
 * Creates a child process, redirects stdout to the specified file,
 * executes the command using execv(), and waits for completion.
 *
 * @param outputfile
 * Full path of the file used to capture command output.
 *
 * @param count
 * Number of arguments supplied through the variable argument list.
 *
 * @param ...
 * Command path followed by its arguments. The first argument must be
 * the absolute path of the executable.
 *
 * @retval true
 * The command completed successfully and returned an exit status of zero.
 *
 * @retval false
 * An error occurred or the command returned a nonzero exit status.
 */
bool do_exec_redirect(const char* outputfile, int count, ...);

/**
 * @brief Determine whether a path is absolute.
 *
 * @param path
 * Path string to examine.
 *
 * @retval true
 * The path is absolute.
 *
 * @retval false
 * The path is relative.
 */
bool is_absolute_path(const char* path);
