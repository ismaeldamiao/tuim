/* ------------------------------------
   Wrapper for the sys_mmap system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_mmap
#define SYSTEMCALL __NR_mmap
#include "systemcall6.S"
