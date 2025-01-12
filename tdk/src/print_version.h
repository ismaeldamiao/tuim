#ifndef PRINT_VERSION_H
#define PRINT_VERSION_H

#include <stdio.h>
#include <tuim.h>

static void print_version(void){
   printf(
      "Tuim version %u\n"
      "Copyright (C) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>\n"
      "|-- version %u.%u\n"
      "`-- configured for the target '%s'\n",
      TUIM_VERSION_MAJOR,
      TUIM_VERSION_MAJOR,
      TUIM_VERSION_MINOR,
      tuim_target()
   );
}


#endif /* PRINT_VERSION_H */
