/****************************** The Tuim Project *******************************
 * Copyright (c) 2024-2025 I.F.F. dos Santos <ismaellxd@gmail.com>
 * Last modified: October 08, 2025.
 ******************************** MIT licensed ********************************/
#include "module.h"

#if !defined(USE_ELF32_TEMPLATE) && !defined(USE_ELF64_TEMPLATE)
   #define USE_ELF32_TEMPLATE
#endif

#define Swap_Half  swap16
#define Swap_Word  swap32
#define Swap_Xword swap64

#if defined(USE_ELF32_TEMPLATE)
   #define Elf(x)    Elf32_##x
   #define Swap_Off  swap32
   #define Swap_Addr swap32
#elif defined(USE_ELF64_TEMPLATE)
   #define Elf(x)    Elf64_##x
   #define Swap_Off  swap64
   #define Swap_Addr swap64
#endif

#define FILE_OFFSET(offset) (void*)(obj + (offset))

static const void *
   Elf(getSHDRentry)(const Elf(Byte) *obj, const Elf(Byte) *section);
static const void *
   Elf(GetSection)(const Elf(Byte) *obj, const Elf(Byte) *section);

/*******************************************************************************
 * Function to parse a ELF file, initialize the tuim_elf structure and load
 * on execution context the program memory segments. For more information read
 * "System V Application Binary Interface", Chapter 5.
 *
 * The ctx variable shall points to a valid tuim_ctx structure. Otherwise the
 * behavior is undefined.
 *
 * The elf variable shall points to a valid tuim_elf structure. Otherwise the
 * behavior is undefined.
 *
 * The function return a error code.
 *
 * NOTE: The current implementation support both ELF32 and ELF64 standard file
 * formats.
 ******************************************************************************/
static int Elf(load)(tuim_ctx *ctx, tuim_elf *elf){
   const uint8_t   *obj = elf->map;
   const Elf(Ehdr) * const ehdr = (void*)obj;
   const Elf(Phdr) *phdr;
   const Elf(Shdr) *shdr;

   Elf(Half) e_phentsize, e_phnum;
   Elf(Word) p_type, i;
   Elf(Off)  e_phoff, sh_offset, p_offset;
   Elf(Addr) p_vaddr, start_vaddr, end_vaddr;
   Elf(Addr) program_image, segment;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word p_filesz, p_memsz, p_align;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword p_filesz, p_memsz, p_align;
   #endif

   bool first_time_here;
   size_t program_size;
   int status;

   const Elf(Byte)
      dynamic_name[] = {46, 100, 121, 110, 97, 109, 105, 99, 00},
      dynstr_name[]  = {46, 100, 121, 110, 115, 116, 114, 0},
      synsym_name[]  = {46, 100, 121, 110, 115, 121, 109, 0};

   /* --- find useful sections --- */

   elf->dyn     = Elf(GetSection)(obj, dynamic_name);
   elf->dynstr  = Elf(GetSection)(obj, dynstr_name);
   shdr          = Elf(getSHDRentry)(obj, synsym_name);
   sh_offset     = Swap_Off(shdr->sh_offset);
   elf->dynsym  = obj + sh_offset;
   elf->sh_info = Swap_Word(shdr->sh_info);
   #if defined(USE_ELF32_TEMPLATE)
      elf->sh_size    = Swap_Word(shdr->sh_size);
      elf->sh_entsize = Swap_Word(shdr->sh_entsize);
   #elif defined(USE_ELF64_TEMPLATE)
      elf->sh_size    = Swap_Xword(shdr->sh_size);
      elf->sh_entsize = Swap_Xword(shdr->sh_entsize);
   #endif

   /* --- Compute the size of program memory image --- */

   e_phoff = Swap_Off(ehdr->e_phoff);
   if(e_phoff == 0) return 0;
   e_phentsize = Swap_Half(ehdr->e_phentsize);
   e_phnum = Swap_Half(ehdr->e_phnum);

   first_time_here = true;

   for(i = 0; i < e_phnum; ++i){
      phdr = FILE_OFFSET(e_phoff + i * e_phentsize);

      p_type = Swap_Word(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_vaddr = Swap_Addr(phdr->p_vaddr);

      if(first_time_here){
         start_vaddr = p_vaddr;
         #if defined(USE_ELF32_TEMPLATE)
            p_align  = Swap_Word(phdr->p_align);
         #elif defined(USE_ELF64_TEMPLATE)
            p_align  = Swap_Xword(phdr->p_align);
         #endif
         first_time_here = false;
      }

      #if defined(USE_ELF32_TEMPLATE)
         end_vaddr = p_vaddr + Swap_Word(phdr->p_memsz);
      #elif defined(USE_ELF64_TEMPLATE)
         end_vaddr = p_vaddr + Swap_Xword(phdr->p_memsz);
      #endif
   }
   program_size = end_vaddr - start_vaddr;

   /* --- Allocate memory for the program image --- */

   status = tuim_alloc(ctx, p_align, program_size, &program_image);
   if(status != TUIM_SUCCESS){
      return status;
   }

   /* --- Read and load loadable segments --- */

   for(i = 0; i < e_phnum; ++i){
      phdr = FILE_OFFSET(e_phoff + i * e_phentsize);

      p_type = Swap_Word(phdr->p_type);
      if(p_type != PT_LOAD) continue;

      p_vaddr = Swap_Addr(phdr->p_vaddr);
      segment = program_image + (p_vaddr - start_vaddr);

      p_offset = Swap_Off(phdr->p_offset);
      #if defined(USE_ELF32_TEMPLATE)
         p_filesz = Swap_Word(phdr->p_filesz);
         p_memsz  = Swap_Word(phdr->p_memsz);
         p_align  = Swap_Word(phdr->p_align);
      #elif defined(USE_ELF64_TEMPLATE)
         p_filesz = Swap_Xword(phdr->p_filesz);
         p_memsz  = Swap_Xword(phdr->p_memsz);
         p_align  = Swap_Xword(phdr->p_align);
      #endif

      /* check if alignment match expected */
      //if((segment % p_align) != 0){
      //   ctx->free(program_image);
      //   return 3;
      //}

      /* copy the file segment to the machine */
      status = tuim_write(ctx, segment, p_filesz, obj + p_offset);
      if(status != TUIM_SUCCESS){
         return status;
      }

      while(p_memsz > p_filesz){
         const uint8_t null = 0;
         (void)tuim_write(ctx, segment + p_filesz, sizeof(uint8_t), &null);
         ++p_filesz;
      }

      /* protect the memory */
      (void)tuim_mprotect(ctx, segment, p_memsz, Swap_Word(phdr->p_flags));
   }

   elf->program_image = program_image;
   elf->program_size = program_size;
   elf->start_vaddr = start_vaddr;
   elf->auxiliary = NULL;
   elf->last.needed = NULL;

   return TUIM_SUCCESS;
}

/*******************************************************************************
 * Helper function to the an entry on the section header table from a given
 * section name, pointed to by section.
 ******************************************************************************/
static const void *
Elf(getSHDRentry)(const Elf(Byte) *obj, const Elf(Byte) *section){
   const Elf(Ehdr) * const ehdr = FILE_OFFSET(0x0);
   const Elf(Shdr) * shdr;
   const Elf(Byte) * shstrtab;
   Elf(Half) e_shstrndx, e_shentsize;
   Elf(Word) sh_name;
   Elf(Off)  e_shoff, sh_offset;
   #if defined(USE_ELF32_TEMPLATE)
      Elf32_Word  e_shnum, i;
   #elif defined(USE_ELF64_TEMPLATE)
      Elf64_Xword e_shnum, i;
   #endif

   e_shoff = Swap_Off(ehdr->e_shoff);
   if(e_shoff == 0) return NULL;
   shdr = FILE_OFFSET(e_shoff);

   e_shstrndx = Swap_Half(ehdr->e_shstrndx);
   if(e_shstrndx == SHN_UNDEF){
      return NULL;
   }else if(e_shstrndx == SHN_XINDEX){
      e_shstrndx = Swap_Word(shdr->sh_link);
   }

   e_shentsize = Swap_Half(ehdr->e_shentsize);
   shdr = FILE_OFFSET(e_shoff + e_shstrndx * e_shentsize);
   sh_offset = Swap_Off(shdr->sh_offset);
   shstrtab = FILE_OFFSET(sh_offset);

   e_shnum = Swap_Half(ehdr->e_shnum);
   if(e_shnum == 0){
      shdr = FILE_OFFSET(e_shoff);
      #if defined(USE_ELF32_TEMPLATE)
         e_shnum = Swap_Word(shdr->sh_size);
      #elif defined(USE_ELF64_TEMPLATE)
         e_shnum = Swap_Xword(shdr->sh_size);
      #endif
   }

   for(i = 1; i < e_shnum; ++i){
      shdr = FILE_OFFSET(e_shoff + i * e_shentsize);
      sh_name = Swap_Word(shdr->sh_name);
      if(strcmp((void*)(shstrtab + sh_name), (void*)section) != 0) continue;
      return shdr;
   }
   return NULL;
}

/*******************************************************************************
 * Helper function to the a section from a given section name,
 * pointed to by section.
 ******************************************************************************/
static const void *
Elf(GetSection)(const Elf(Byte) *obj, const Elf(Byte) *section){
   const Elf(Shdr) *shdr = Elf(getSHDRentry)(obj, section);
   return FILE_OFFSET(Swap_Off(shdr->sh_offset));
}

#undef Swap_Half
#undef Swap_Word
#undef Swap_Xword
#undef Swap_Off
#undef Swap_Addr

#undef FILE_OFFSET
#undef Elf
