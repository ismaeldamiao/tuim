/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 06, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

/* TODO: Give a utility for this. */
static tuim_ctx this = {
   .tuim_va = tuim_va
};
tuim_ctx *tuim_thiscontext = &this;

/*******************************************************************************
 * Function to create a new context of execution. This is the reference
 * implementation for Unix, for more details see the file "module.h".
 *
 * The function shall allocate memory for the tuim_ctx structure and initialize
 * it. Then a new process is created, the .text section of that process have a
 * trap instruction and a system call instruction, both suitable to the be used
 * with ptrace to control the process.
 ******************************************************************************/
static int tuim_newcontext(
   const char *tuim_home, const char *ld_library_path, tuim_ctx **_ctx
){
   static const char enoent[] = "Can't replace the child process image";
   static const char eunknown[] = "Unknown error";

   int status, fd;
   size_t size;
   struct_regs regs;
   pid_t pid;
   tuim_ctx *ctx;

   /**
    * Allocate memory for and initialize the tuim_ctx structure.
    */

   if(tuim_home == NULL) tuim_home = "";
   if(ld_library_path == NULL) ld_library_path = "";

   size = sizeof(tuim_ctx) + 2;
   size += strlen(tuim_home);
   size += strlen(ld_library_path);

   ctx = malloc(size);
   if(ctx== NULL){
      *_ctx = NULL;
      return TUIM_ENOMEM;
   }
   *_ctx = ctx;
   ctx->tuim_va = tuim_va;
   ctx->elf_list = NULL; /* empty for now */
   ctx->not_expecting_exec = false;
   ctx->tuim_home = (char*)ctx + sizeof(tuim_ctx);
   ctx->ld_library_path = ctx->tuim_home + strlen(tuim_home) + 1;
   (void)strcpy(ctx->tuim_home, tuim_home);
   (void)strcpy(ctx->ld_library_path, ld_library_path);

   #ifndef NDEBUG
      (void)printf("tuim: Making a new process...\n");
   #endif

   pid = fork();
   if((int)pid < 0){
      return 1;
   }

   /**
    * The child, forked from the current process, call PTRACE_TRACEME
    * and turn itself a tracee, then their process image is replaced by
    * little_image.
    */
   if((int)pid == 0){
      char file_name[BUFFER_SIZE];

      (void)ptrace(PTRACE_TRACEME, (pid_t)0, NULL, NULL);
      (void)strcpy(file_name, ctx->tuim_home);
      (void)strcat(file_name, "/tmp/seed");

      fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0755);
      if(fd == -1){
         exit(TUIM_ENOENT);
      }

      if((int)write(fd, little_image, sizeof(little_image)) == -1){
         exit(TUIM_ENOENT);
      }
      (void)close(fd);

      {
         char *const argv[] = { "little_image", NULL };
         char *const envp[] = { NULL };
         if(execve(file_name, argv, envp) == -1){
            exit(TUIM_ENOEXEC);
         }
      }
   }

   /**
    * Tracer waits until the child. On error the child exits, on success the
    * child is sinal SIGTRAP.
    */
   waitpid(pid, &status, __WALL);
   if(WIFEXITED(status)){
      (void)kill(pid, SIGKILL);
      ctx->strerror = enoent;
      return WEXITSTATUS(status);
   }

   /**
    * The child shall be killed if the tracer terminates.
    */
   if(ptrace(PTRACE_SETOPTIONS, pid, NULL,
      PTRACE_O_EXITKILL | PTRACE_O_TRACEEXIT) == -1){
      (void)kill(pid, SIGKILL);
      ctx->strerror = eunknown;
      return TUIM_EUNKNOWN;
   }

   /**
    * Finally, some information shall be stored. The entry point adress and
    * the address of the instruction to do a system call. Both get from the
    * program counter register.
    */
   if(ptrace(PTRACE_GETREGS, pid, NULL, regs) == -1){
      (void)kill(pid, SIGKILL);
      ctx->strerror = eunknown;
      return TUIM_EUNKNOWN;
   }

   ctx->pid = pid;
   ctx->entry = regs[program_counter];
   ctx->syscall = regs[program_counter] + trap_size;

   #ifndef NDEBUG
      (void)printf("tuim:    pid: %d\n", pid);
      (void)printf("tuim:    entry: 0x%X\n", ctx->entry);
      (void)printf("tuim:    system call: 0x%X", ctx->syscall);
      (void)puts("");
   #endif

   return TUIM_SUCCESS;
}
