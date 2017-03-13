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
 *      - Pretty-printing functions for binary
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
  memset(error_message, 0, ERROR_MESSAGE_MAX_LENGTH);
  int prefix_length = strlen(error_prefix);

  // Format and print an error message with perror
  strncpy(error_message, error_prefix, prefix_length);
  strncat(error_message, message,
          ERROR_MESSAGE_MAX_LENGTH - (prefix_length + 1));

  // TODO: Check ERRNO and get the actual OS error if necessary
  fprintf(stderr, "%s", error_message);
  exit(exit_value);
}

/*
 * safe_malloc - allocate a zero'd out memory segment, exiting on failure
 *
 * Args:
 *  unsigned int size - the number of bytes to allocate
 */
void *safe_malloc(unsigned int size) {
  void *ptr;

  // Check for memory allocation success
  ptr = malloc(size);
  if (ptr == NULL)
    fatal_error(1, "Memory allocation failed");

  // Zero out freshly allocated memory
  memset(ptr, 0, (size_t) size);

  return ptr;
}

/*
 * safe_calloc - allocate some number of sequential memory segments in the heap
 *
 * Args:
 *  unsigned int count - the number of sequential segments to allocate
 *  unsigned int size - the number of bytes to allocate per segment
 */
void *safe_calloc(unsigned int count, unsigned int size) {
  void *ptr;

  // Check for memory allocation success
  ptr = calloc(count, size);
  if (ptr == NULL)
    fatal_error(1, "Memory allocation failed");

  return ptr;
}

/*
 * fbindump - print representation of binary to a file
 *
 * Args:
 *  int fd  - File descriptor of file to which to dump
 *  unsigned char *data - binary data to be printed
 *  unsigned int length - Number of bytes to print
 */
void fbindump(int fd, unsigned char *data, unsigned int length) {
  unsigned char byte;
  unsigned int i, j;

  // Get a file object from the filedescriptor
  FILE *fp = fdopen(fd, "w");

  // Loop through each byte in the data
  for (i = 0; i < length; i++) {
    byte = data[i];
    fprintf(fp, "%02x ", data[i]);  // Print hex byte

    // Modulus math to get the hex printing right
    if (((i % 16) == 15) || (i == length - 1)) {
      for (j = 0; j < 15 - (i % 16); j++)
        fprintf(fp, "   ");
      fprintf(fp, "| ");

      // Print the character representation of each byte on the line
      for (j = (i - (i % 16)); j <= i; j++) {
        byte = data[j];
        if ((byte > 31) && (byte < 127))
          fprintf(fp, "%c", byte);
        else
          fprintf(fp, ".");
      }

      fprintf(fp, "\n"); // End of the dump line (each of which is 16 bytes)
    }
  }
}



