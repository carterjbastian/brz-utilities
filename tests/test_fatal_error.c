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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
  int fdes_out[2];  // File descriptors for STDOUT pipe
  int fdes_err[2];
  fd_set fds;  // File descriptor set for select
  int max_fd;
  struct timeval timeout;  // Timeout
  int ready;  // ready value returned by select
  char stderr_buff[4096];
  char stdout_buff[4096];
  ssize_t count;  // Number of bytes read by call to read
  pid_t pid;

  stderr_buff[0]= '\0';
  stdout_buff[0] = '\0';

  // Open new pipes for the child process to write to
  if (pipe(fdes_out) == -1) {
    perror("Pipe");
    return 0;
  }
  if (pipe(fdes_err) == -1) {
    perror("Pipe");
    return 0;
  }

  // Initialize the filedescriptor sets
  FD_ZERO(&fds);

  FD_SET(fdes_out[0], &fds);
  FD_SET(fdes_err[0], &fds);

  // Find the largest filedescriptor of the two
  max_fd = (fdes_out[0] > fdes_err[0]) ? fdes_out[0] : fdes_err[0];

  // Create the time out such that each attempted read will be wait less
  // than the specified maximum wait time.
  timeout.tv_sec = 0;
  timeout.tv_usec = UNITTEST_TIMEOUT_MSECS;

  // For a new process to fail
  pid = fork();

  if (pid < 0) {
    return 0; // TODO: error message logging while it failed
  } else if (pid == 0) { // CHILD:
    // Duplicate the file descriptor
    while ((dup2(fdes_out[1], STDOUT_FILENO) == -1) &&
        (errno == EINTR)) {}

    while ((dup2(fdes_err[1], STDERR_FILENO) == -1) &&
        (errno == EINTR)) {}

    // Close the pipes
    close(fdes_out[1]);
    close(fdes_out[0]);
    close(fdes_err[1]);
    close(fdes_err[0]);

    // Exit fatally
    fatal_error(1, "Testing error\n");

    return 0; // If you make it here, the fatal failing didn't work

  } else { // PARENT:
    // Wait for the process to return no matter what
    wait(0);

    // Check if the redirected output pipes are ready to be read
    ready = select(max_fd + 1, &fds, NULL, NULL, &timeout);
    if (ready == -1) {
        perror("Read stdout");
        return 0;
    } else if (ready > 0) {
      // Capture output and break
      if (FD_ISSET(fdes_out[0], &fds))
        count = read(fdes_out[0], stdout_buff, sizeof(stdout_buff));

      if (FD_ISSET(fdes_err[0], &fds))
        count = read(fdes_err[0], stderr_buff, sizeof(stderr_buff));
    }

    // Close the open pipes
    close(fdes_out[0]);
    close(fdes_err[0]);

    return 1;
  }
  return 1;
}
