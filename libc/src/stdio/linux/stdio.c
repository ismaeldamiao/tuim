#include <stdio.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2
struct __file {
   /* File Descriptor:
      Must be first, in order to allow a safe cast of FILE* to
      a pointer to the file descriptor.  */
   int fd;
};

static FILE
   stdin_ = {
      STDIN_FILENO
   },
   stdout_ = {
      STDOUT_FILENO
   },
   stderr_ = {
      STDERR_FILENO
   };
FILE
   * const __stdin = &stdin_,
   * const __stdout = &stdout_,
   * const __stderr = &stderr_;
