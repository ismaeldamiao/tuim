/* ------------------------------------
   Wrapper for the lseek system call.
   * Part of Tuim Project.
   * Last modified: July 14, 2025.
------------------------------------ */
#define WRAPPER    lseek
#define SYSTEMCALL __NR_lseek
#include "wrappers/systemcall3.S"
