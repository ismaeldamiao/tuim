/* ------------------------------------
   Wrapper for the sys_munmap system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_munmap
#define SYSTEMCALL __NR_munmap
#include "systemcall2.S"
