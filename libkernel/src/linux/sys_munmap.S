/* ------------------------------------
   Wrapper for the munmap system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    munmap
#define SYSTEMCALL __NR_munmap
#include "systemcall2.S"
