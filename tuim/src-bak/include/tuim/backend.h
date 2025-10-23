/** ----------------------------------------------------------------------------
   @file
      tuim/backend.h
   @brief
      The backend interface for Tuim.

      The <tuim/backend.h> header file contain function prototypes used to write
      a backend for the Tuim's ELF Interpreter.
   @author
      I.F.F. dos Santos <ismaellxd@gmail.com>
   @copyright

   Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the “Software”), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

----------------------------------------------------------------------------- */
#ifndef TUIM_BACKEND_H
#define TUIM_BACKEND_H
#include <tuim.h>

/**
   @brief
      Set up a execution environment on the target machine.

      The "vm" at the end may refer to Virtual Memory since this step
      generary means to prepare a new process with their own virtual
      address space when the target machine is the host machine itself.
      The "vm" at the end may also refer to Virtual Machine since this step
      generay means to create a new virtual machine instance when
      the host machine can't execute the required ISA.
   @param ptr
      The pointer pointed to by ptr
      shall point to a object valid to be used with others backend
      functions, the information stored on that object is unspecified.
   @param home 
   @return
      On success the function shall return TUIM_SUCESS.
      On error return a TUIM_* value. */
int tuim_target_new_vm(void **ptr, const char *home);

/**
   @brief
      Release a execution environment on the target machine.

      Delete the execution environt and release all resources
      previous created by tuim_target_new_vm.
   @param ptr
      The backend pointer.
*/
void tuim_target_delete_vm(void *ptr);

/**
   @brief
      Allocate memory on the target machine.
   @param ptr
      The backend pointer
   @return
      This function return the begin address of the allocated memory
      region. */
tuim_addr tuim_target_aligned_alloc(void *ptr, tuim_size alignment, tuim_size size);

/**
   @brief
      Read the target machine's memory
   @param ptr
      The backend pointer
   @param dest
      A pointer to the object that shall get the readed bytes
   @param src
      The address to start reading
   @param size
      The amount of bytes to read
   @return */
int tuim_target_read(void *ptr, void *dest, tuim_addr src, tuim_size size);

/**
   @brief
      Write on the target machine's memory
   @param ptr
      The backend pointer
   @param dest
      The address to write on
   @param src
      A pointer to the object that shall be written
   @param size
      The amount of bytes to write, i.e., the object size
   @return */
int tuim_target_write(void *ptr, tuim_addr dest, const void *src, tuim_size size);

/**
   @brief
      Copy a object on the target machine
   @param ptr
      The backend pointer
   @param dest
      TODO: Complete
   @param src
      TODO: Complete
   @param size
      TODO: Complete
   @return 
      TODO: Complete */
int tuim_target_memcpy(void *ptr, tuim_addr dest, tuim_addr src, tuim_size size);

/**
   @brief
      Get the actual state of the target machine registers.
   @param ptr
      The backend pointer
   @param new
      TODO: Complete
   @param old
      TODO: Complete
   @return
      TODO: Complete */
int tuim_target_registers(void *ptr, void *new, void *old);

/**
   @brief
      Run the target machine
   @param ptr
      The backend pointer
   @param address
      TODO: Complete
   @return */
int tuim_target_jump(void *ptr, tuim_addr address);

/**
   @brief
      Set up the target machine initial state.
   @param ptr
      The backend pointer
   @param argv
      TODO: Complete
   @param envp
      TODO: Complete
   @return
      TODO: Complete */
int tuim_target_exec_init(void *ptr, char *const argv[], char *const envp[]);

/**
   @brief
      Read and release the machine final state.
   @param ptr
      The backend pointer
   @param exit_status
      TODO: Complete
   @return
      TODO: Complete */
int tuim_target_exec_fini(void *ptr, int *exit_status);

#endif /* TUIM_BACKEND_H */
