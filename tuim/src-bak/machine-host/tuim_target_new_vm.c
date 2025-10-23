#include <stddef.h>
#include <stdint.h>
#include <tuim.h>

#define DEBUG 0

#if __STDC_VERSION__ < 201112L
   #define thread_local
#elif  (__STDC_VERSION__ >= 201112L) && (__STDC_VERSION__ < 202311L)
   #define thread_local _Thread_local
#endif

#if defined(__STDC_VERSION__)
#if __STDC_VERSION__ >= 202311L
#if __has_include(<tuim/kernel.h>)
   #define tuim_environment
#endif // __has_include(<tuim/kernel.h>)
#endif /* __STDC_VERSION__ < 202311L */
#endif /* defined(__STDC_VERSION__) */

#if defined(tuim_environment)
   #include <tuim/arch.h>
   #include <tuim/kernel.h>
#else
   #include "arch.h"
   #include "kernel.h"
   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      #include <string.h>
      #include <signal.h>
      #include <stdlib.h>
      #include <unistd.h>
      #include <fcntl.h>
      #include <sys/stat.h>
      #include <sys/types.h>
      #include <sys/wait.h>
      #include <sys/ptrace.h>
      #include <stdlib.h>
   #endif
#endif

#if   tuim_kernel == tuim_linux || \
      tuim_kernel == tuim_xnu
   /* On Unix-like systems I need a little executable to start writting on it,
      here is the text section in ARM:
         BKPT #0
         SVC  #0
         MOV  pc, lr
      Generate by compiling for the target and use
         clang -c little_program.s
         ld.lld -s --gc-sections --no-omagic --no-rosegment -z nognustack -z nosectionheader -o little_program little_program.o
         xxd -i little_program
   */
   const uint8_t little_program[] = {
  0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x28, 0x00, 0x01, 0x00, 0x00, 0x00,
  0x74, 0x00, 0x01, 0x00, 0x34, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x00, 0x05, 0x34, 0x00, 0x20, 0x00, 0x02, 0x00, 0x28, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x34, 0x00, 0x00, 0x00,
  0x34, 0x00, 0x01, 0x00, 0x34, 0x00, 0x01, 0x00, 0x40, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x01, 0x00, 0x80, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x70, 0x00, 0x20, 0xe1,
  0x00, 0x00, 0x00, 0xef, 0x0e, 0xf0, 0xa0, 0xe1, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
   };
#endif

#include "asm.h"

int tuim_target_new_vm(void **ptr, const char *home){
   int tuim_target_registers(void *ptr, void *new, void *old);

   #if DEBUG
      int printf(const char *, ...), puts(const char *);
      printf("Making a new process\n");
   #endif

   #if   tuim_kernel == tuim_linux || \
         tuim_kernel == tuim_xnu
      /* on unix-like systems that have ptrace the process is
         forked and the child is traced by the parent */
      struct backend { pid_t pid; uintptr_t entry; uintptr_t syscall; } *back;
      struct_regs regs;
      int status;
      pid_t pid;
      int fd;

      pid = fork();
      if((int)pid < 0){
         return 1;
      }

      if((int)pid == 0){
         char file_name[1024];

         /* the child call ptrace(PTRACE_TRACEME) and turn itself a tracee,
            then a little executable is writen and used to replace the current
            process image, the code executed is expected to enter a SIGTRAP */
         ptrace(PTRACE_TRACEME, (pid_t)0, NULL, NULL);

         strcpy(file_name, home);
         strcat(file_name, "/tmp/seed");

         fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
         if(fd == -1){
            exit(TUIM_ENOENT);
         }

         if((int)write(fd, little_program, sizeof(little_program)) == -1){
            exit(TUIM_ENOENT);
         }
         close(fd);

         {
            int execve(const char *path, char *const argv[], char *const envp[]);
            char *const argv[] = { "little_program", NULL };
            char *const envp[] = { NULL };
            if(execve(file_name, argv, envp) == -1){
               exit(TUIM_ENOEXEC);
            }
         }
      }

      /* first synchronize with the child, wait until a SIGTRAP signal */
      waitpid(pid, &status, __WALL);
      if(WIFEXITED(status)){
         #if DEBUG
            printf("   exited: %d\n", WEXITSTATUS(status));
         #endif
         kill(pid, SIGKILL);
         return WEXITSTATUS(status);
      }

      #if DEBUG
         printf("   ... process %d waiting to do a task", pid);
         puts("");
      #endif

      if(ptrace(PTRACE_SETOPTIONS, pid, NULL,
         PTRACE_O_EXITKILL | PTRACE_O_TRACEEXIT) == -1){
         kill(pid, SIGKILL);
         return 1;
      }

      if(tuim_target_registers(&pid, NULL, regs) != TUIM_SUCCESS){
         kill(pid, SIGKILL);
         return 1;
      }

      back = malloc(sizeof(struct backend));
      if(back == NULL){
         kill(pid, SIGKILL);
         return TUIM_ENOMEM;
      }
      back->pid = pid;
      back->entry = regs[program_counter];
      back->syscall = regs[program_counter] + trap_size;

      #if DEBUG
         printf("   pid: %d\n", pid);
         printf("   entry: 0x%X", back->entry);
         puts("");
      #endif

      *ptr = back;
   #elif tuim_kernel == tuim_windows_nt
   #endif

   return TUIM_SUCCESS;
}
