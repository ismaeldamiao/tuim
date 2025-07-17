/* ------------------------------------
   Wrapper for the sys_open system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    sys_open
#define SYSTEMCALL __NR_open
#include "systemcall3.S"
