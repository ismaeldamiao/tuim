#include "tuim_utils.h"

/* ------------------------------------
   Initialization of external variables.
   * Part of tuim project.
   * Last modified: Octubre 10, 2024.
------------------------------------ */

static char tuim_private_str[512];

unsigned int tuim_error;
char *tuim_error_filename = NULL;
char *tuim_str = tuim_private_str;
elf_list *tuim_loaded = NULL;
