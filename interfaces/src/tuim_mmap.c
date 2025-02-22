/* *****************************************************************************
   MIT License

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
***************************************************************************** */
#include "tuim.h"
#include "main.h"
/* ------------------------------------
   This function map the a ELF file in the process virtual address space
   or load the content of a already loaded file in the memory.
   * Part of Tuim Project.
   * Last modified: February 21, 2025.
------------------------------------ */

#if defined(_POSIX_C_SOURCE)
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <Windows.h>
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */

int tuim_mmap(const uint8_t *buf, void *structure){

   if(isELF(buf)){ /* Load the program memory image ------------------------- */
      struct image_s *image = structure;
      bool first_time_here = true;
      const uint8_t *obj = buf;
      uint8_t **program = &(image->program);
      size_t *sz = &(image->sz), *needed_num = &(image->needed_num);
      Elf_Addr end_vaddr, *start_vaddr = &(image->start_vaddr);
      const Elf_Ehdr *ehdr = (Elf_Ehdr*)obj;
      Elf_Dyn *dynamic = NULL;

      /* Compute the size of program memory image */
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_phnum); ++i){
         Elf_Phdr *phdr;

         phdr = Phdr_entry(i);
         if(phdr->p_type == PT_DYNAMIC){
            dynamic = (Elf_Dyn*)(obj + phdr->p_offset);
            continue;
         }
         if(phdr->p_type != PT_LOAD) continue;

         if(first_time_here){
            *start_vaddr = phdr->p_vaddr;
            first_time_here = false;
         }
         end_vaddr = phdr->p_vaddr + (Elf_Addr)(phdr->p_memsz);
      }

      *sz = (size_t)(end_vaddr - *start_vaddr);

      /* Allocate memory for the program image */
#if defined(_POSIX_C_SOURCE)
      *program = mmap(
         NULL,
         *sz,
         PROT_READ | PROT_WRITE | PROT_EXEC,
         MAP_PRIVATE | MAP_ANONYMOUS,
         -1, (off_t)0
      );
      if(*program == MAP_FAILED){
         return TUIM_ENOMEM;
      }
#elif defined(_WIN32)
      *program = VirtualAlloc(NULL, *sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
      if(*program == NULL){
         return TUIM_ENOMEM;
      }
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */

      /* Read and load loadable segments */
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_phnum); ++i){
         void *segment;
         Elf_Phdr *phdr;

         phdr = Phdr_entry(i);

         if(phdr->p_type != PT_LOAD) continue;

         segment = *program + (phdr->p_vaddr - *start_vaddr);

         _memcpy(segment, obj + phdr->p_offset, phdr->p_filesz);

         if(phdr->p_memsz > phdr->p_filesz){
            _memset(
               (uint8_t*)segment + phdr->p_filesz, 0,
               phdr->p_memsz - phdr->p_filesz
            );
         }

         /* Protect the memory */
#if defined(_POSIX_C_SOURCE)
         {
            int prot;
            if(phdr->p_flags & PF_X) prot |= PROT_EXEC;
            if(phdr->p_flags & PF_W) prot |= PROT_WRITE;
            if(phdr->p_flags & PF_R) prot |= PROT_READ;
            mprotect(segment, phdr->p_memsz, prot);
         }
#elif defined(_WIN32)
         DWORD oldProtect;
         if(phdr->p_flags & PF_X) {
            if(!VirtualProtect(segment, phdr->p_memsz, PAGE_EXECUTE_READ, &oldProtect)) {
               return TUIM_ENOMEM;
            }
         } else if(phdr->p_flags & PF_W) {
            if(!VirtualProtect(segment, phdr->p_memsz, PAGE_READWRITE, &oldProtect)) {
               return TUIM_ENOMEM;
            }
         } else if(phdr->p_flags & PF_R) {
            if(!VirtualProtect(segment, phdr->p_memsz, PAGE_READONLY, &oldProtect)) {
               return TUIM_ENOMEM;
            }
         }
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */
      }

      *needed_num = SIZE_C(0);
      if(dynamic != NULL){
         for(Elf_Dyn *dyn = dynamic; dyn->d_tag != DT_NULL; ++dyn){
            if(dyn->d_tag == DT_NEEDED) ++(*needed_num);
         }
      }
   }else{ /* Map the file --------------------------------------------------- */
#if defined(_POSIX_C_SOURCE)
      struct file_s *file = structure;
      int fd;
      struct stat s;

      fd = open(string(buf), O_RDONLY);
      if(fd == -1){
         return TUIM_ENOENT;
      }

      fstat(fd, &s);
      file->sz = s.st_size;
      {
         off_t offset = (off_t)0;
         file->obj = mmap(
            NULL, file->sz, PROT_READ, MAP_PRIVATE, fd, offset
         );
         if(file->obj == MAP_FAILED){
            close(fd);
            return TUIM_ENOMEM;
         }
      }
      close(fd);
#elif defined(_WIN32)
      struct file_s *file = structure;
      HANDLE hFile;
      LARGE_INTEGER fileSize;

      hFile = CreateFileA(string(buf), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
      if(hFile == INVALID_HANDLE_VALUE){
         return TUIM_ENOENT;
      }

      if(GetFileSizeEx(hFile, &fileSize) == 0){
         CloseHandle(hFile);
         return TUIM_ENOENT;
      }

      file->sz = fileSize.QuadPart;
      {
         void* mapping = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
         if(mapping == NULL){
            CloseHandle(hFile);
            return TUIM_ENOMEM;
         }

         file->obj = MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, 0);
         if(file->obj == NULL){
            CloseHandle(hFile);
            CloseHandle(mapping);
            return TUIM_ENOMEM;
         }

         CloseHandle(mapping);
      }
      CloseHandle(hFile);
#else
#error Unknown target OS
#endif /* defined(_POSIX_C_SOURCE) */
   }
   return TUIM_SUCCESS;
}
