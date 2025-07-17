/* ------------------------------------
   Wrapper for the sys_close system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_close
#define SYSTEMCALL __NR_close
#include "systemcall1.S"
