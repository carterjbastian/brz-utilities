/*
 * test_utils.c     Unit tests for test_utilities
 *
 * Author:          Carter J. Bastian
 *                  February, 2017
 *
 * Description:
 *    Builds a simple unit testing framework for my utilities.
 *
 *    To add a new test:
 *      - Define the unit test as a function in some file in this directory
 *      - Add the test function declaration in test_utils.h
 *      - Add a reference to the test and its name in global variable test_list
 *
 *    Guidelines:
 *      All files in the test_list should be run and return a 1 on success or a
 *      0 on failure. If testing for a failure case, a subprocess should be run
 *      as a pthread.
 *
 *      A general success case for <FUNCTION> should be named test_<FUNCTION>.
 *      A more specific test of <FEATURE> should be named
 *      test_<FUNCTION>_<FEATURE>.
 */
// System Includes
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Local Includes
#include "../brz_utils.h"
#include "test_utils.h"

/*
 * The test table is the set of definitions of list of unit tests.
 * WARNINGS:
 *  - The order in which the items in the table are listed matters! This must
 *    be the same as the order in which the tokens are enumerated in
 *    the test_token typedef (from test_utils.h).
 *
 *  - As in the test_token typedef, it is imperative for indexing that
 *    ROOT_UNIT_TEST be the first item defined in the table, and
 *    END_UNIT_TEST must be the last item defined in the table.
 */
static unit_test test_table[] = {
  { ROOT_UNIT_TEST, test_root_unit_test, "test_root_unit_test" },
  { FATAL_ERROR_CORRECT_MESSAGE, test_fatal_error_correct_message, "test_fatal_error_correct_message" },
  { FATAL_ERROR_CODE, test_fatal_error_code, "test_fatal_error_code" },
  { FATAL_ERROR_MESSAGE_MAX, test_fatal_error_message_max, "test_fatal_error_message_max" },
  { SAFE_MALLOC, test_safe_malloc, "test_safe_malloc" },
  { SAFE_MALLOC_ZEROS, test_safe_malloc_zeros, "test_safe_malloc_zeros" },
  { SAFE_MALLOC_FAILS, test_safe_malloc_fails, "test_safe_malloc_fails" },
  { END_UNIT_TESTS, NULL, NULL },
};

/* Macros for accessing the test_table via tokens */
#define UNIT_TEST_INDEX(X)      ( (X) - ROOT_UNIT_TEST )
#define UNIT_TEST_FUNCTION(X)   ( test_table[ UNIT_TEST_INDEX((X)) ].function )
#define UNIT_TEST_NAME(X)       ( test_table[ UNIT_TEST_INDEX((X)) ].name )


// Public Functions

/*
 * create_uproc - run a function as a process and return its output
 *
 * Args:
 *    token - the test token for the current unit test. AKA the symbolic value
 *      for the current test in its unit_test struct in the test table
 *      EG. ROOT_UNIT_TEST
 *
 *    utest_func - a function to be run in its own process. Similar in usage
 *      to a pthread, a uproc function should accept a generic pointer (void *)
 *      and should return nothing. Rather, this function should exit with a
 *      status code.
 *
 *    args - a pointer to some utest_func specific data structure which, when
 *      deconstructed, has the parameters to pass into utest_func.
 *
 * Return Values:
 *    Under normal circumstances, returns a pointer to a dynamically allocated
 *    struct containing the exit code, stdout output, and stderr output of the
 *    process. In the event of an internal error, returns NULL and prints with
 *    perror.
 *
 * Caution:
 *    It is the responsibility of the caller to free the dynamically allocated
 *    uproc_status pointer, as well as its dynamically allocated members
 *    (stderr_buff and stdout_buff).
 *
 * TODO: write a cleanup function for uprocs.
 * TODO: figure out non-standard return values from create_uproc
 */
uproc_status *create_uproc(int token, void (*utest_func)(void *), void *args) {
  int fdes_out[2];  // File descriptors for STDOUT pipe
  int fdes_err[2];  // File descriptors for STDERR pipe
  fd_set fds;       // File descriptor set for select
  int max_fd;       // Maximum file descriptor for select
  struct timeval timeout;   // Timeout for select
  int pipes_ready;          // Pipe readability indicator returned by select
  char stderr_buff[UPROC_OUTPUT_MAX_LEN + 1];  // Buffer for output from stderr
  char stdout_buff[UPROC_OUTPUT_MAX_LEN + 1];  // Buffer for output from stdout
  ssize_t stdout_len = 0;       // Number of bytes read from uproc's stdout
  ssize_t stderr_len = 0;       // Number of bytes read from uproc's stderr
  pid_t pid;        // Process ID to track the
  int status;       // Status of the child process returned by wait
  int uproc_exit_code;      // The exit code of the child process
  uproc_status *retval;     // struct with uproc output to be returned
  int stdout_junk_len;      // Length of the junk printed to stdout by main

  // Zero out the buffers
  memset(stderr_buff, 0, UPROC_OUTPUT_MAX_LEN + 1);
  memset(stdout_buff, 0, UPROC_OUTPUT_MAX_LEN + 1);

  // Open new pipes for the child process to write to
  if (pipe(fdes_out) == -1) {
    perror("Pipe");
    return NULL;
  }
  if (pipe(fdes_err) == -1) {
    perror("Pipe");
    return NULL;
  }

  // Initialize the filedescriptor sets
  FD_ZERO(&fds);
  FD_SET(fdes_out[0], &fds);
  FD_SET(fdes_err[0], &fds);

  // Find the largest filedescriptor of the two
  max_fd = (fdes_out[0] > fdes_err[0]) ? fdes_out[0] : fdes_err[0];

  // Create a timeout with a somewhat arbitrary amount of wait time
  timeout.tv_sec = 0;
  timeout.tv_usec = UNITTEST_TIMEOUT_MSECS;

  // For a new process to fail
  pid = fork();

  if (pid < 0) {
    // Check for piping error
    perror("Bad Pipe");
    return NULL;
  } else if (pid == 0) {
    // CHILD process: Setup the output pipes and run utest_func
    // Duplicate file descriptors for stdout and stderr
    while ((dup2(fdes_out[1], STDOUT_FILENO) == -1) &&
        (errno == EINTR)) {}
    while ((dup2(fdes_err[1], STDERR_FILENO) == -1) &&
        (errno == EINTR)) {}

    // Close pipes in advanced
    close(fdes_out[1]);
    close(fdes_out[0]);
    close(fdes_err[1]);
    close(fdes_err[0]);

    // Run the unittest (which should exit on its own)
    (*utest_func)(args);

  } else { // PARENT:
    // Wait for the process to return
    // TODO: Use alarm system call to timeout with varying lengths
    status = 0;
    if (wait(&status) == -1) {
      perror("wait system call failed");
      return NULL;
    }

    // Check that the process exited and collect its exit code
    if (!(WIFEXITED(status))) {
      perror("uproc did not exit normally");
      return NULL;
    }
    uproc_exit_code = WEXITSTATUS(status);

    // Check if the redirected output pipes are ready to be read
    pipes_ready = select(max_fd + 1, &fds, NULL, NULL, &timeout);
    if (pipes_ready == -1) {
        perror("Read stdout");
        return NULL;
    } else if (pipes_ready > 0) {
      // Capture output and break
      if (FD_ISSET(fdes_out[0], &fds))
        stdout_len = read(fdes_out[0], stdout_buff, sizeof(stdout_buff));

      if (FD_ISSET(fdes_err[0], &fds))
        stderr_len = read(fdes_err[0], stderr_buff, sizeof(stderr_buff));
    }


    // Close the open pipes
    close(fdes_out[0]);
    close(fdes_err[0]);

    // Create the structure to return
    retval = (uproc_status *) malloc(sizeof(uproc_status));
    retval->exit_code = uproc_exit_code;
    retval->stdout_buff = NULL;   // String pointers default to null pointer
    retval->stderr_buff = NULL;

    if (stdout_len > (18 + strlen(UNIT_TEST_NAME(token)))) {
      // Because STDOUT is forked from the main method of test_utils.c, it
      // always prints the string "  running test <test name>..."
      // Flush these characters from the buffer.
      stdout_junk_len = 18 + strlen(UNIT_TEST_NAME(token));
      stdout_len -= stdout_junk_len;

      retval->stdout_buff = (char *) malloc(sizeof(char) * (stdout_len + 1));
      strncpy(retval->stdout_buff, stdout_buff + stdout_junk_len , stdout_len);
      retval->stdout_buff[stdout_len] = '\0';  // Force null termination
    }
    if (stderr_len > 0) {
      retval->stderr_buff = (char *) malloc(sizeof(char) * (stderr_len + 1));
      strncpy(retval->stderr_buff, stderr_buff, stderr_len);
      retval->stderr_buff[stderr_len] = '\0';  // Force null termination
    }

    // Return the structure with information about the child process
    return retval;
  }
  perror("Unexpected lack of return value in uproc_create");
  return NULL;
}
/*
 * test_root_unit_test - tests that the unit test framework is working
 */
int test_root_unit_test() {
  return 1;
}

/*
 * When running as main, run all of the unit tests and assert their success
 */
int main(int argc, char **argv) {
  // Local variabes
  int test_passed = 0;
  int total_passes = 0;
  int total_fails = 0;
  int token = -1; // Identifying token for the current unit test
  int (*run_test)(void); // Function pointer to current unit test

  // Build a record of all of the unit test results
  int test_results[END_UNIT_TESTS - ROOT_UNIT_TEST];

  // Initialize all results to indicate that they weren't run
  for (int i = 0; i < END_UNIT_TESTS - ROOT_UNIT_TEST; i++)
    test_results[i] = -1;

  // Print welcome banner
  printf("================================================================\n");
  printf("Starting unit tests:\n");

  // Do a second pass, running the unit tests and recording their results
  for (token = ROOT_UNIT_TEST; token < END_UNIT_TESTS; token++) {
    run_test = UNIT_TEST_FUNCTION(token);
    // WARNING: if you change this print statement, you must also change
    // the segment of code that copies the stdout pipe to a string in uproc.
    printf("  Running test %s...", UNIT_TEST_NAME(token));

    test_passed = run_test();
    test_results[UNIT_TEST_INDEX(token)] = test_passed;

    if (test_passed) {
      printf("  test passed!\n");
      total_passes++;
    } else {
      printf("  test failed!\n");
      total_fails++;
    }
  }

  // Print the unit test result summary
  printf("================================================================\n");
  printf("\nSummary of %d total tests run:\n", END_UNIT_TESTS - ROOT_UNIT_TEST);
  printf("\tPasses: %d, Failures: %d\n",
          total_passes,
          total_fails);

  // Print either a list of failed tests or a happy message
  if (total_fails) {
    printf("\nFailed Tests:\n");

    // Loop through each unit test result, and print the failures
    for (token = ROOT_UNIT_TEST; token < END_UNIT_TESTS; token++) {
      if (!test_results[UNIT_TEST_INDEX(token)])
        printf("  - %s()\n", UNIT_TEST_NAME(token));
    }
  } else {
    printf("\nAll unit tests passed :)\n");
  }

  return(0);
}
