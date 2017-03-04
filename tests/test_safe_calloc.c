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
 * test_safe_calloc - test contiguous allocation succeeds in normal case
 */
int test_safe_calloc() {
  unsigned int count = 10;
  unsigned int size = 50;   // Allocate a 50-character string
  void *segment = safe_calloc(count, size);

  // Assert the calloc succeeded
  if (!segment)
    return 0;

  // Assert the data segment
  for (int i = 0; i < (count * size); i++) {
    if (*((char *)segment + i) != '\0')
      return 0;
  }

  return 1;
}
