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
   This is a ELF interpreter for the Tuim project.
   * Part of Tuim Project.
   * Last modified: February 20, 2025.
------------------------------------ */

static bool tuim_HaveCorrectAttributes(void *_ehdr, bool is_executable);
static void tuim_LoadProgram(
   void *_obj,
   Elf_Addr *start_vaddr, size_t *sz, size_t *needed_num, void *_program
);
static void tuim_FillElfStructure(elf_s *elf);
static void tuim_GetDep(uint8_t **obj_name, elf_s *elf);
static void relocate(elf_s *elf);


int tuim_interpreter(uint8_t *file_path, elf_s **main_file){
   int tuim_errno;

   DBG("Loading executable '%s'.\n", string(file_path));
   /* Step 0 -------------------------------------------------------------------
      Map the file and load it
      In this routine I map the main ELF file in the virtual address space,
      parse the ELF file and load in memory the program on such file.
      Then I repeat the process for all its dependencies, and for dependencies
      of dependencies, and so on. Information is stored in a double linked list,
      the order I load files is suitable for easy execution of initialization
      and termination functions.
   -------------------------------------------------------------------------- */
   {
      elf_s *father, *elf = NULL;
      struct file_s file;
      struct image_s image;
      elf_s **needed;

      file.name = file_path;

      /* map_new_elf:
         [[in]]  obj_name
         [[out]] filesz
         [[out]] obj      (or NULL on failure) and set tuim_errno */
      begin_map_new_elf: {
         static bool is_home_used = false;
         uint8_t obj_path[2048];

         /* Routine to get the object path from its name */
         if((file.name == file_path) || haveslash(file.name)){
            /* The first file must be a absolute or relative path, I do not find
               for it in any place. */
            _strcpy(obj_path, file.name);
         }else{
            /* If the name is not a absolute or relative path and I loading a
               dependency, then I need to find for the file first on directories
               describeded by the environment variable LD_LIBRARY_PATH, if I
               cannot find it on such directories then I find for it on
               <TUIM_HOME>/lib, the default place. */
            static size_t i = SIZE_C(0);

            if(i < tuim_ld_library_path_len){
               const uint8_t ascii_slash[] = {ASCII_SLASH, UINT8_C(0x00)};
               /* Get the next entry on ld_library_path symbol table */
               _strcpy(obj_path, tuim_env[LD_LIBRARY_PATH] + i);
               _strcat(obj_path, ascii_slash);
               _strcat(obj_path, file.name);
               i += _strlen(tuim_env[LD_LIBRARY_PATH] + i) + SIZE_C(2);
            }else{
               /* Use TUIM_HOME */
               if(is_home_used || (tuim_env[TUIM_HOME] == NULL)){
                  file.obj = NULL;
                  tuim_errno = TUIM_ENOENT;
                  goto end_map_new_elf;
               }
               is_home_used = true;

               _strcpy(obj_path, tuim_env[TUIM_HOME]);
               _strcat(obj_path, ascii("/lib/"));
               _strcat(obj_path, file.name);
               i = SIZE_C(0);
            }
         }

         /* Such call map the ELF in the current process address space. */
         tuim_errno = tuim_mmap(obj_path, &file);
         if(tuim_errno != TUIM_SUCCESS){
            if(tuim_errno == TUIM_ENOENT){
               goto begin_map_new_elf;
            }else if(tuim_errno != TUIM_ENOMEM){
               goto error;
            }else{
               /* FIXME: There are better approaches to handle unknown errors. */
               *(volatile Elf_Addr*)NULL;
            }
         }

         /* I do only basic checks in order to validade the file. */
         /* FIXME: If such errors occurs in the first file I need to
            goto error;
            --- I know this is easy but is a problem for other day */
         if(!isELF(file.obj)){
            tuim_munmap(&file, NULL);
            goto begin_map_new_elf;
         }
         if(!tuim_HaveCorrectAttributes(file.obj, (elf == NULL))){
            /* FIXME: Is not the case that the first file
               is allways a executable. */
            tuim_munmap(&file, NULL);
            goto begin_map_new_elf;
         }

         is_home_used = false;
         DBG("   '%s' maped at %p\n", obj_path, file.obj);
      } end_map_new_elf:

      if(file.obj == NULL){
         goto error;
      }

      /* Such call load the program in the memory and fill the image structure.
         Also, and this is important, compute the number of dependencies,
         such number is used bellow to allocate memory for the elf structure. */
      tuim_mmap(file.obj, &image);

      if(image.program == NULL){
         tuim_munmap(&file, NULL);
         goto error;
      }

      DBG("   '%s' loaded at %p\n", file.name, image.program);

      {
         void *mem;
         mem = malloc(sizeof(elf_s) + sizeof(elf_s*) * (image.needed_num + 1));
         if(mem == NULL){
            tuim_errno = TUIM_ENOMEM;
            goto error;
         }
         if(*main_file == NULL){
            *main_file = elf = father = mem;
            needed = elf->dep;
            elf->previous = NULL;
            elf->next = NULL;
         }else{
            elf->next = mem;
            elf->next->previous = elf;
            elf = elf->next;
            elf->next = NULL;
            *needed = elf, ++needed;
         }
      }

      elf->file  = file;
      elf->image = image;

      /* Fill other members of the ELF structure, I preffer do it here instead
         of in the tuim_relocate function. */
      tuim_FillElfStructure(elf);

      get_next_obj_name: {

         /* This function set in the first argument the next dependency
            I need to interpret. */
         tuim_GetDep(&(file.name), father);
         if(file.name != NULL){

            // Check if already loaded
            for(elf_s *elf = *main_file; elf != NULL; elf = elf->next){
               if(_strcmp(file.name, elf->file.name) == 0){
                  goto get_next_obj_name;
               }
            }
            DBG("Loading shared object '%s'.\n", file.name);
            goto begin_map_new_elf;
         }
         needed = NULL;
         father = father->next;
         if(father != NULL){
            needed = father->dep;
            goto get_next_obj_name;
         }
      }
   }

   for(elf_s *elf = *main_file; elf != NULL; elf = elf->next){
      DBG("Relocating '%s'\n", elf->file.name);
      relocate(elf);
   }

   return TUIM_SUCCESS;

   error:
   tuim_free(*main_file);
   return tuim_errno;
}

static void tuim_FillElfStructure(struct elf_s *elf){
   uint8_t
      *obj = elf->file.obj,
      *program = elf->image.program;
   Elf_Addr start_vaddr = elf->image.start_vaddr;
   Elf_Ehdr *ehdr = (Elf_Ehdr*)obj;
   Elf_Dyn *dynamic = NULL;
   Elf_Shdr *dynsym;

   /* Find the entry point */
   elf->entry = (void(*)(void))(program + (ehdr->e_entry - start_vaddr));

   /* Find for the Dynamic Section */
   {
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_phnum); ++i){
         Elf_Phdr *phdr;

         phdr = Phdr_entry(i);
         if(phdr->p_type != PT_DYNAMIC) continue;

         dynamic = (Elf_Dyn*)(obj + phdr->p_offset);
         break;
      }

      if(dynamic == NULL) return;
      elf->dynamic = dynamic;
   }

   // Find for the dynamic symbol table
   {
      for(size_t i = SIZE_C(0); i < (size_t)(ehdr->e_shnum); ++i){
         Elf_Shdr *shdr;

         shdr = Shdr_entry(i);
         if(shdr->sh_type != SHT_DYNSYM) continue;

         elf->dynsym.sym = (Elf_Sym*)(obj + shdr->sh_offset);
         elf->dynsym.str = obj + Shdr_entry(shdr->sh_link)->sh_offset;
         elf->dynsym.first_non_local = (size_t)(shdr->sh_info);
         elf->dynsym.symnum =
            (size_t)(shdr->sh_size) / sizeof(Elf_Sym);
         break;
      }
   }
}

static void tuim_GetDep(uint8_t **obj_name, elf_s *elf){
   static uint8_t *obj = NULL, *strtab;
   static Elf_Dyn *dyn;

   if(obj != elf->file.obj){
      obj = elf->file.obj;
      dyn = elf->dynamic;
      strtab = elf->dynsym.str;
   }

   if(dyn == NULL){
      *obj_name = NULL;
      return;
   }

   for(;dyn->d_tag != DT_NULL; ++dyn){
      if(dyn->d_tag == DT_NEEDED){
         *obj_name = strtab + dyn->d_un.d_val;
         ++dyn;
         return;
      }
   }

   *obj_name = NULL;
   return;
}

/* Processor dependent implementations -------------------------------------- */
#if defined(__ARM_ARCH)
   #if __ARM_ARCH >= 8
      /* ------------------------------------
      AARCH64
      ------------------------------------ */
      #include "aarch64.c"
   #elif __ARM_ARCH <= 7
      /* ------------------------------------
      AARCH32
      ------------------------------------ */
      #include "aarch32.c"
   #endif /* __ARM_ARCH >= 8 */
#elif defined(__x86_64__)
   /* ------------------------------------
   AMD64
   ------------------------------------ */
   #include "amd64.c"
#elif defined(__i386__)
   /* ------------------------------------
   i386
   ------------------------------------ */
      #error "Not implemented yet."
#elif defined(__riscv)
   #if __riscv_xlen == 64
      /* ------------------------------------
      RISC-V 64
      ------------------------------------ */
      #error "Not implemented yet."
   #elif __riscv_xlen == 32
      /* ------------------------------------
      RISC-V 32
      ------------------------------------ */
      #error "Not implemented yet."
   #endif /* __riscv_xlen == 64 */
#else
   #error "Processor architecture not supported."
#endif /* defined(__ARM_ARCH) */

#include "tuim_mmap.c"
#include "tuim_munmap.c"
