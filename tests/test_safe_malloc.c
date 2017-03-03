/*
 * test_safe_malloc.c      Unit tests for ../brz_utils.c#safe_malloc
 *
 * Author:      Carter J. Bastian
 *              February, 2017
 *
 * Description:
 *    Unit tests for the safe_malloc function in brz_utils.c.
 *
 */
// Local Includes
#include "../brz_utils.h"
#include "test_utils.h"

// Public Function Definitions
/*
 * test_safe_malloc - test allocation succeeds at allocation in normal case
 */
int test_safe_malloc() {
  unsigned int size = 50;   // Allocate a 50-character string
  unsigned long int data; // Allocate a pointer for this data

  char *test_string = safe_malloc(size);
  unsigned long int *data_ptr = safe_malloc(sizeof(data));

  // Assert that both mallocs succeeded
  if (!(test_string && data_ptr))
    return 0;

  return 1;
}

/*
 * test_safe_malloc_zeros - test allocation is auto-zero'd out
 */
int test_safe_malloc_zeros() {
  unsigned int size = 100;  // Allocate a 100-byte segment of memory
  void *segment = safe_malloc(size);


  for (int i = 0; i < size; i++) {
    if ((*((char *)segment + i)) != '\0')
      return 0;
  }
  return 1;
}
