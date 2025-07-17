/* ------------------------------------
   Wrapper for the sys_mprotect system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_mprotect
#define SYSTEMCALL __NR_mprotect
#include "systemcall3.S"
