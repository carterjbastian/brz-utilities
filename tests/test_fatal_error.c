/*
 * test_fatal_error.c      Tests for ../brz_utils.c#fatal_error
 *
 * Author:                 Carter J. Bastian
 *                         February, 2017
 *
 * Description:
 *    Unit tests for the fatal_error utility method
 *
 */

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Includes
#include "../brz_utils.h"
#include "test_utils.h"

// Private Function Declarations
struct fatal_error_params {
  int code;
  const char *msg;  // Ensure the string is statically allocated
} fatal_error_params;

void fatal_error_uproc(void *args);

// Unit Tests
/*
 * test_fatal_error_correct_message - test fatal_error exits with the  message
 *
 * Test that the fatal error message coming from a thread that exits with
 * fatal error is as expected.
 */
int test_fatal_error_correct_message() {
  uproc_status *retval;
  struct fatal_error_params *args;
  const char *error_message = "This is an error message such wow\n";
  char *intended_error;
  char *returned_message;
  int match;

  // Initialize the parameter structure
  args = malloc(sizeof(fatal_error_params));
  if (!args) {
    perror("malloc failed");
    return 0;
  }
  args->code = 1;
  args->msg = error_message;

  // Create the intended error message
  int message_len = strlen(error_message) + strlen(error_prefix);
  intended_error = (char *) malloc(message_len + 1);
  intended_error[message_len] = '\0';
  strncat(intended_error, error_prefix, strlen(error_prefix));
  strncat(intended_error, error_message, strlen(error_message));

  // Run the uproc and collect its exit status
  retval = create_uproc(FATAL_ERROR_CORRECT_MESSAGE,
                        &fatal_error_uproc, (void *) args);

  // Collect the exit status
  if (retval) {
    returned_message = retval->stderr_buff;
  } else {
    perror("uproc Failed");
    free(args);
    return 0;
  }

  // Compare the stderr output with what was intended
  if (strlen(intended_error) != strlen(returned_message))
    match = -1;
  else
    match = strncmp(intended_error, returned_message, strlen(intended_error));

  // Check that there is no stdout output
  // TODO: print error message explaining this error
  if (retval->stdout_buff)
    match = -1;

  // Clean up memory from uproc return
  if (retval->stderr_buff)
    free(retval->stderr_buff);
  if (retval->stdout_buff)
    free(retval->stdout_buff);
  if (retval)
    free(retval);

  // Return test results
  if (match != 0)
    return 0;
  else
    return 1;
}

/*
 * test_fatal_error_code - test fatal_error error code is correct
 *
 * Test that exiting a thread with fatal_error causes the exit code to be set
 * to whatever value is passed as the first parameter to fatal_error
 */
int test_fatal_error_code() {
  int test_codes[] = {0, 1, 10, 100, 255};
  int test_code_len = 5;
  int exit_status = -1;
  uproc_status *retval;
  struct fatal_error_params *args;

  // Initialize the parameter structure
  args = malloc(sizeof(fatal_error_params));
  if (!args) {
    perror("malloc failed");
    return 0;
  }
  args->msg = "Failing in test_fatal_error_code\n";

  for (int i = 0; i < test_code_len; i++) {
    // Run the uproc and collect its exit status
    args->code = test_codes[i];
    retval = create_uproc(FATAL_ERROR_CODE, &fatal_error_uproc, (void *) args);

    // Collect the exit status
    if (retval) {
      exit_status = retval->exit_code;
    } else {
      perror("uproc Failed");
      free(args);
      return 0;
    }

    // Clean up memory from uproc return
    if (retval->stderr_buff)
      free(retval->stderr_buff);
    if (retval->stdout_buff)
      free(retval->stdout_buff);
    if (retval)
      free(retval);

    // Return if test failed
    if (exit_status != test_codes[i]) {
      free(args);
      return 0;
    }
  }
  // All error codes returned were correct. Cleanup and return
  free(args);
  return 1;
}

/*
 * test_fatal_error_message_max - test fatal_error with overflowing message
 */
int test_fatal_error_message_max() {
  uproc_status *retval;
  struct fatal_error_params *args;
  char *long_error_message;
  int long_message_length;
  char *returned_message;
  int success = 0;

  // Create the long error message to be more than the max length
  long_message_length = ERROR_MESSAGE_MAX_LENGTH * 2;
  long_error_message = (char *) malloc(long_message_length + 1);
  memset((void *)long_error_message, 'A', long_message_length);
  long_error_message[long_message_length] = '\0'; // Null termination

  // Initialize the parameter structure
  args = malloc(sizeof(fatal_error_params));
  if (!args) {
    perror("malloc failed");
    return 0;
  }
  args->code = 1;
  args->msg = long_error_message;

  // Run the uproc and collect its exit status
  retval = create_uproc(FATAL_ERROR_MESSAGE_MAX,
                        &fatal_error_uproc, (void *) args);

  // Collect the exit status
  if (retval) {
    returned_message = retval->stderr_buff;
  } else {
    perror("uproc Failed");
    free(args);
    return 0;
  }

  // Check that the length of the error message is not more than the max length
  if (returned_message) {
    if (strlen(returned_message) != (ERROR_MESSAGE_MAX_LENGTH - 1)) {
      success = -1;
    }
  } else {
    success = -1;
  }

  // Check that there is no stdout output
  // TODO: print error message explaining this error
  if (retval->stdout_buff)
    success = -1;

  // Clean up memory from uproc return
  if (retval->stderr_buff)
    free(retval->stderr_buff);
  if (retval->stdout_buff)
    free(retval->stdout_buff);
  if (retval)
    free(retval);

  // Return test results
  if (success != 0)
    return 0;
  else
    return 1;
}

// Private functions
void fatal_error_uproc(void *args_p) {
  // Cast the arguments to the param struct
  struct fatal_error_params *args = (struct fatal_error_params *)args_p;

  // Call fatal_error with the correct params
  fatal_error(args->code, (char *)args->msg);
}
