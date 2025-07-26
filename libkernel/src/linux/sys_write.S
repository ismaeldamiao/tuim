/* ------------------------------------
   Wrapper for the write system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    write
#define SYSTEMCALL __NR_write
#include "systemcall3.S"
