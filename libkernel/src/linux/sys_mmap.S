/* ------------------------------------
   Wrapper for the mmap system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    mmap
#define SYSTEMCALL __NR_mmap
#include "systemcall6.S"
