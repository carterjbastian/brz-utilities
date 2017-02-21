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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

// Local Includes
#include "../brz_utils.h"
#include "test_utils.h"

// Global Variables

// Private Function Declarations
void *fatal_exit_process(void *fatal_exit_data);

// Unit Tests
/*
 * test_fatal_error_correct_message - test fatal_error exits with the  message
 *
 * Test that the fatal error message coming from a thread that exits with
 * fatal error is as expected.
 */
int test_fatal_error_correct_message() {
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

  pid_t pid;

  for (int i = 0; i < test_code_len; i++) {
    pid = 0;
    int status = 0;

    // Fork a new process to fail
    pid = fork();

    if (pid < 0) {
      // Ensure fork succeeded
      return 0; // TODO: error message logging why it failed
    } else if (pid == 0) {
      // CHILD: call fatal_exit with return code
      char *message = "Failing in test_fatal_error_code";
      fatal_error(test_codes[i], message);
    } else {
      // PARENT: wait for child to finish and check exit code
      status = 0;
      if (wait(&status) == -1)
        return 0;

      // Check that the exit status is correct
      if (!(WIFEXITED(status) && WEXITSTATUS(status) == test_codes[i]))
        return 0;
    }
  }
  // All error codes returned were correct
  return 1;
}

/*
 * test_fatal_error_message_max - test fatal_error with overflowing message
 */
int test_fatal_error_message_max() {
  return 1;
}
