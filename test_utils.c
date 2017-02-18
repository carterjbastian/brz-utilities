/*
 * test_utils.c      Unit tests for test_utilities
 *
 * Author:      Carter J. Bastian
 *              February, 2017
 *
 * Description:
 *    Builds a simple unit testing framework for my utilities.
 *
 *    To add a new test:
 *      - Add the test function declaration
 *      - Add a reference to the test and its name in global variable test_list
 *      - Create the function definition that is the test
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
#include <stdio.h>

// Local Includes
#include "brz_utils.h"

// Macros and constants
#define FATAL_THREAD_ERROR -5

/*
 * The test table is the set of definitions of list of unit tests.
 * WARNINGS:
 *  - The order in which the items in the table are listed matters! This must
 *    be the same as the order in which the tokens are enumerated in
 *    the test_token typedef (from test_utils.h).
 *
 *  - As in the test_token typedef, it is imperative for indexing that
 *    ROOT_UNIT_TEST be the first item defined in the table.
 */
static unit_test test_table[] = {
  { ROOT_UNIT_TEST, test_root_unit_test, "test_root_unit_test" },
  { FATAL_ERROR, test_fatal_error, "test_fatal_error" },
  { FATAL_ERROR_MESSAGE_MAX, test_fatal_error_message_max, "test_fatal_error_message_max" },
  { SAFE_MALLOC, test_safe_malloc, "test_safe_malloc" },
  { SAFE_MALLOC_ZEROS, test_safe_malloc_zeros, "test_safe_malloc_zeros" },
  { SAFE_MALLOC_FAILS, test_safe_malloc_fails, "test_safe_malloc_fails" },
}

// Private Function Declarations
void fail_with_fatal_error();

// Unit Tests
/*
 * test_fatal_error - test fatal_error exits with error code and message
 */
int test_fatal_error() {
  return 1;
}

/*
 * test_fatal_error_message_max - test fatal_error with overflowing message
 */
int test_fatal_error_message_max() {
  return 1;
}

/*
 * test_safe_malloc - test allocation succeeds at allocation in normal case
 */
int test_safe_malloc() {
  return 1;
}

/*
 * test_safe_malloc_zeros - test allocation is auto-zero'd out
 */
int test_safe_malloc_zeros() {
  return 1;
}

/*
 * test_safe_malloc_fails - test allocation fails appropriately
 */
int test_safe_malloc_fails() {
  return 1;
}


// Private Function Definitions
void fail_with_fatal_error() {
  return
}

/*
 * When running as main, run all of the unit tests and assert their success
 */
int main(int argc, char **argv) {

}
