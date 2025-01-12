#include "tuim_utils.h"

/* ------------------------------------
   Initialization of external variables.
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

static char tuim_private_str[512];

unsigned int tuim_errno;
char *tuim_error_filename = NULL;
char *tuim_str = tuim_private_str;
elf_list *tuim_loaded = NULL;

void tuim_perror(const char *s){
   fprintf(stderr, "%s: %s", s, tuim_strerror(tuim_errno));
}

char* tuim_strerror(unsigned int errnum){
   char *str[] = {
      "Unknown error.\n",
      "Unsuficient memory.\n",
      "Error reading the file.\n",
      "Invalid ELF file.\n",
      "Unsupported machine.\n",
      "System error.\n",
      "Not a dynamic library.\n",
      "Unsupported runtime.\n",
      "Symbol not found.\n",
      "Bad memory layout.\n",
      "Can't find the entry point.\n",
      "Unknown execution type.\n",
      "Can't create a process.\n",
      "Not a executable.\n"
   };
   return str[errnum];
}
