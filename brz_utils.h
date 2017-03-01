/*
 * brz_utils.h      Function declarations for a brz-utilities
 *
 * Author:      Carter J. Bastian
 *              February, 2017
 *
 * Description:
 *    Declarations for my personal c utility functions.
 *
 * Attributions:
 *    Some of these functions are adapted (or directly reused) versions of
 *    functions originally presented in "hacking; the art of exploitation" by
 *    Jon Erickson.
 */
#ifndef BRZ_UTILS_H_
#define BRZ_UTILS_H_

// Constant and Macro Definitions
#define ERROR_MESSAGE_MAX_LENGTH 1024
extern const char *error_prefix;

// Public Function Declarations
void fatal_error(int exit_value, char *message);

void *safe_malloc(unsigned int size);

#endif // BRZ_UTILS_H_

