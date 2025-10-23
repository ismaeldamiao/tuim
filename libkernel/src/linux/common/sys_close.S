/* ------------------------------------
   Wrapper for the close system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    close
#define SYSTEMCALL __NR_close
#include "wrappers/systemcall1.S"
