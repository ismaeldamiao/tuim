/* ------------------------------------
   Wrapper for the open system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    open
#define SYSTEMCALL __NR_open
#include "wrappers/systemcall3.S"
