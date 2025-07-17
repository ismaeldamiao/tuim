/* ------------------------------------
   Wrapper for the sys_lseek system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_lseek
#define SYSTEMCALL __NR_lseek
#include "systemcall3.S"
