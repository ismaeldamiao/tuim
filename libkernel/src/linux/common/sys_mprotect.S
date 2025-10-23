/* ------------------------------------
   Wrapper for the mprotect system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    mprotect
#define SYSTEMCALL __NR_mprotect
#include "wrappers/systemcall3.S"
