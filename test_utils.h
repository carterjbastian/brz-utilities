/*
 * test_utils.h      ...
 *
 * Author:      Carter J. Bastian
 *              February, 2017
 *
 * Description:
 *
 */
#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_

// Datatype Declarations
typedef struct {
  int token;
  void *function;
  char *name;
} unit_test;

/* Enum with the tokens for each unit test above */
typedef enum {
  ROOT_UNIT_TEST, // WARNING: this has to stay the first token defined
  FATAL_ERROR,
  FATAL_ERROR_MESSAGE_MAX,
  SAFE_MALLOC,
  SAFE_MALLOC_ZEROS,
  SAFE_MALLOC_FAILS,
} test_token;

// Unit Test declarations
/* Test for the testing framework */
int test_root_unit_test();

/* Tests for fatal_error */
int test_fatal_error();
int test_fatal_error_message_max();

/* Tests for safe_malloc */
int test_safe_malloc();
int test_safe_malloc_zeros();
int test_safe_malloc_fails();

#endif // TEST_UTILS_H_
