#include <stddef.h>
#include <stdio.h>

extern char **__libc_environment;

int main(int argc, char **argv){
   for(;*argv != NULL; ++argv)
      puts(*argv);
   for(char **envp = __libc_environment; *envp != NULL; ++envp)
      puts(*envp);
   return 0;
}

extern void __libc_main(typeof(&main));
void program(void){ __libc_main(main); }
