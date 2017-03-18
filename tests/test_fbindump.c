/*
 * test_fbindump.c         Tests for ../brz_utils.c#fbindump
 *
 * Author:                 Carter J. Bastian
 *                         February, 2017
 *
 * Description:
 *    Unit tests for the fbindump utility method to pretty print binary
 *
 */

// System Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Local Includes
#include "../brz_utils.h"
#include "test_utils.h"

// Test Data (shouts out to Chance's verse on Hello. Shit was straight fire.)
char *message = "Hello rap game, nǐ hǎo, how you durrn'? "
                "I done took my time, now it's time to take my turn\r\n";

char *intended_output =
        "48 65 6c 6c 6f 20 72 61 70 20 67 61 6d 65 2c 20 | Hello rap game, \n"
        "6e c7 90 20 68 c7 8e 6f 2c 20 68 6f 77 20 79 6f | n.. h..o, how yo\n"
        "75 20 64 75 72 72 6e 27 3f 20 49 20 64 6f 6e 65 | u durrn'? I done\n"
        "20 74 6f 6f 6b 20 6d 79 20 74 69 6d 65 2c 20 6e |  took my time, n\n"
        "6f 77 20 69 74 27 73 20 74 69 6d 65 20 74 6f 20 | ow it's time to \n"
        "74 61 6b 65 20 6d 79 20 74 75 72 6e 0d 0a       | take my turn..\n";

// Private Function Declarations
struct fbindump_params {
  int fd;
  const char *msg;  // Ensure the string is statically allocated
  int length;
} fatal_error_params;

void fbindump_uproc(void *args);

/*
 * test_fbindump - test fbindump printing to stdout on a standard message
 */
int test_fbindump() {
  uproc_status *retval;
  struct fbindump_params *args;
  char *returned_message;
  int success = 0;

  // Initialize the parameter structure
  args = malloc(sizeof(fatal_error_params));
  if (!args) {
    perror("malloc failed");
    return 0;
  }
  args->fd = STDOUT_FILENO;
  args->msg = message;
  args->length = strlen(message);

  // Run the uproc and collect its exit status
  retval = create_uproc(FBINDUMP,
                        &fbindump_uproc, (void *) args);

  // Collect the exit status
  if (retval) {
    returned_message = retval->stdout_buff;
  } else {
    perror("uproc Failed");
    free(args);
    return 0;
  }

  // Check that the standard output was the correct length
  if (returned_message) {
    if (strlen(returned_message) != strlen(intended_output)) {
      success = -1;
    }
  } else {
    success = -1;
  }

  // Check that the standard output has the correct value
  if (success >= 0 && strcmp(returned_message, intended_output) != 0) {
    success = -1;
  }

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
void fbindump_uproc(void *args_p) {
  // Cast the arguments to the param struct
  struct fbindump_params *args = (struct fbindump_params *)args_p;

  // Call fbindump with the correct params
  fbindump(args->fd, (unsigned char *)args->msg, args->length);
  exit(0); // Exit on success
}
