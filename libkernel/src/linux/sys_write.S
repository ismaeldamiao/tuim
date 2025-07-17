/* ------------------------------------
   Wrapper for the sys_write system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_write
#define SYSTEMCALL __NR_write
#include "systemcall3.S"
