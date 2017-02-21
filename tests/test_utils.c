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
#include <stdio.h>

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
