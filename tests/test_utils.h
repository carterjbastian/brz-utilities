/*
 * test_utils.h     Datatypes and declarations for unit testing framework
 *
 * Author:          Carter J. Bastian
 *                  February, 2017
 *
 * Description:
 *    Contains the definition of a unit test as a token, function pointer, and
 *    name. Also enumerates the unit tests, and consists of the declarations of
 *    all unit tests defined in test_utils.c
 */
#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

// Datatype and constant declarations
typedef struct {
  int token;
  int (*function)(void);  // A pointer to a unit test function
  char *name;
} unit_test;

#define PIPE_READ_MAX_ATTEMPTS 3
#define UNITTEST_TIMEOUT_MSECS 10000
/*
 * Enum with the tokens for each unit test above.
 * CAUTION: new tokens must be added between ROOT_UNIT_TEST and END_UNIT_TESTS
 * for indexing purposes. For consistent results, add new unit tests directly
 * before END_UNIT_TESTS.
 */
typedef enum {
  ROOT_UNIT_TEST,   // WARNING: this has to stay the first token defined
  FATAL_ERROR_CORRECT_MESSAGE,
  FATAL_ERROR_CODE,
  FATAL_ERROR_MESSAGE_MAX,
  SAFE_MALLOC,
  SAFE_MALLOC_ZEROS,
  SAFE_MALLOC_FAILS,
  END_UNIT_TESTS,   // WARNING: this must be the last token defined
} test_token;

/*
 * Unit Test declarations
 * All unit tests for the project should be declared here and ordered
 * by the file which contains their definition.
 */
/* Test for the testing framework (test_utils.c) */
int test_root_unit_test();

/* Tests for fatal_error (test_fatal_error.c) */
int test_fatal_error_correct_message();
int test_fatal_error_code();
int test_fatal_error_message_max();

/* Tests for safe_malloc (test_safe_malloc.c) */
int test_safe_malloc();
int test_safe_malloc_zeros();
int test_safe_malloc_fails();

#endif // TEST_UTILS_H_
