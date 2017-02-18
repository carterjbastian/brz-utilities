/*
 * brz_utils.c      Implementations of utilities defined in brz_utils.h
 *
 * Author:          Carter J. Bastian
 *                  February, 2017
 *
 * Description:
 *    Implementations of personal c utility functions declared in brz_utils.h
 *    Includes:
 *      - Error handling
 *      - Error-checking wrappers for memory allocation
 *
 * Attributions:
 *    Some of these functions are adapted (or directly reused) versions of
 *    functions originally presented in "hacking; the art of exploitation" by
 *    Jon Erickson.
 */

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Includes
#include "brz_utils.h"

// Constants
const char *error_prefix = "[!!] Fatal Error: ";

// Public Function Definitions
/*
 * fatal_error - prints error message and exits with an error code
 *
 * Args:
 *  int exit_value - error value the process should return
 *  char *message - the error message to print to the console
 *
 */
void fatal_error(int exit_value, char *message) {
  char error_message[ERROR_MESSAGE_MAX_LENGTH];
  int prefix_length = strlen(error_prefix);

  // Format and print an error message with perror
  strncpy(error_message, error_prefix, prefix_length);
  strncat(error_message, message, ERROR_MESSAGE_MAX_LENGTH - prefix_length);
  perror(error_message);

  exit(exit_value);
}

void *safe_malloc(unsigned int size) {
  void *ptr;

  // Check for memory allocation success
  ptr = malloc(size);
  if (ptr == NULL)
    fatal_error(-1, "Memory allocation failed");

  // Zero out freshly allocated memory
  memset(ptr, 0, (size_t) size);

  return ptr;
}
