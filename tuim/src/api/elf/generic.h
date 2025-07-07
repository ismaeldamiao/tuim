/* *****************************************************************************
   Copyright (c) 2024 FennelFoxxo
   Copyright (c) 2025 I.F.F. dos Santos <ismaellxd@gmail.com>

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
#ifndef _ELF_GENERIC_H
#define _ELF_GENERIC_H

#include "endian.h"
/* ------------------------------------
   This header file contain static function to parse ELF files.
   * Module designed for the Tuim Project.
   * Last modified: May 07, 2025.
------------------------------------ */

#include "elf.h"

#if __STDC_VERSION__ < 202311L
#define true 1
#define false 0
typedef int bool;
#endif /* __STDC_VERSION__ < 202311L */

typedef uint8_t  Elf_Byte;
typedef uint16_t Elf_Half;
typedef uint32_t Elf_Word;
typedef int32_t  Elf_Sword;
typedef uint64_t Elf_Xword;
typedef int64_t  Elf_Sxword;

typedef struct {
   Elf_Byte e_ident[EI_NIDENT];
   Elf_Half e_type;
   Elf_Half e_machine;
   Elf_Word e_version;
   Elf_Xword e_entry;
   Elf_Xword e_phoff;
   Elf_Xword e_shoff;
   Elf_Word e_flags;
   Elf_Half e_ehsize;
   Elf_Half e_phentsize;
   Elf_Half e_phnum;
   Elf_Half e_shentsize;
   Elf_Half e_shnum;
   Elf_Half e_shstrndx;
   // Members below this aren't part of the elf header, but they're useful to have here
    
   // These 5 members will 0 if their associated section is not present (or if there are 0 sections)
   uint64_t                string_table_offset;        // byte offset of .shstrtab data referenced by e_shstrndx
   uint64_t                symbol_table_offset;        // byte offset of .symtab data
   uint64_t                symbol_string_table_offset; // byte offset of .strtab data referenced by .symtab
   uint64_t                symbol_entry_size;          // Size in bytes of each symbol entry
   uint64_t                symbol_num;                 // Number of symbols
   
   // If the number of sections >= SHN_LORESERVE, then e_shnum will be 0 and the true number of sections is stored elsewhere
   // This value accounts for that case, and should be used when determining how many sections are actually present
   uint64_t                true_shnum;
   
   // Same as previous member - accounts for edge case if string table section index >= SHN_LORESERVE
   // Set to 0 if there are no sections
   uint64_t                true_shstrndx;
   
   uint64_t                elf_size;                   // Total size in bytes of elf data
} Elf_Ehdr;

typedef struct {
   Elf64_Word sh_name;
   Elf_Word sh_type;
   Elf_Xword sh_flags;
   Elf_Xword sh_addr;
   Elf_Xword sh_offset;
   Elf_Xword sh_size;
   Elf_Word sh_link;
   Elf_Word sh_info;
   Elf_Xword sh_addralign;
   Elf_Xword sh_entsize;

   const char*         name;   // Calculated from sh_name and .shstrtab offset. Will always point to valid string
   uint64_t            index;    // Section index, useful if section was obtained by name
} Elf_Shdr;

typedef struct {
   Elf_Word p_type;
   Elf_Word p_flags;
   Elf_Xword p_offset;
   Elf_Xword p_vaddr;
   Elf_Xword p_paddr;
   Elf_Xword p_filesz;
   Elf_Xword p_memsz;
   Elf_Xword p_align;

   uint64_t            index;
} Elf_Phdr;

typedef struct {
   Elf_Word st_name;
   Elf_Byte st_info;
   Elf_Byte st_other;
   Elf_Half st_shndx;
   Elf_Xword st_value;
   Elf_Xword st_size;

   Elf_Byte       st_bind;        // Upper nibble of st_info
   Elf_Byte       st_type;        // Lower nibble of st_info
   Elf_Byte st_visibility;  // Lower 2 bits of st_other
   
   const Elf_Byte*         name;   // Calculated from st_name and .strtab offset. Will always point to valid string
   uint64_t                index;    // Symbol index, useful if symbol was obtained by name
} Elf_Sym;

typedef struct {
  Elf64_Sxword d_tag;
  union {
      Elf_Xword d_val;
      Elf_Xword d_ptr;
  } d_un;
} Elf_Dyn;

typedef struct {
   Elf_Ehdr ehdr;
   Elf_Shdr *shdr;
   Elf_Phdr *phdr;
   Elf_Dyn  *dyn;
   Elf_Sym  *sym;
} Elf_Object;

/* The Elf_magic function returns true if the magic number is correct,
   else returns false. */
static inline bool Elf_magic(const Elf_Byte *buffer){
   const Elf_Byte *e_ident = buffer;
   return (
      (e_ident[EI_MAG0] == ELFMAG0) &&
      (e_ident[EI_MAG1] == ELFMAG1) &&
      (e_ident[EI_MAG2] == ELFMAG2) &&
      (e_ident[EI_MAG3] == ELFMAG3)
   );
}

static void Elf32_Parse_Ehdr(Elf_Ehdr *hdr, const Elf32_Ehdr *buffer){
   bool is_lsb = (buffer->e_ident[EI_DATA] == ELFDATA2LSB);

   hdr->e_version    = convert_endian_32(buffer->e_version,   is_lsb);
   hdr->e_type       = convert_endian_16(buffer->e_type,      is_lsb);
   hdr->e_machine    = convert_endian_16(buffer->e_machine,   is_lsb);
   hdr->e_entry      = convert_endian_32(buffer->e_entry,     is_lsb);
   hdr->e_phoff      = convert_endian_32(buffer->e_phoff,     is_lsb);
   hdr->e_shoff      = convert_endian_32(buffer->e_shoff,     is_lsb);
   hdr->e_flags      = convert_endian_32(buffer->e_flags,     is_lsb);
   hdr->e_ehsize     = convert_endian_16(buffer->e_ehsize,    is_lsb);
   hdr->e_phentsize  = convert_endian_16(buffer->e_phentsize, is_lsb);
   hdr->e_phnum      = convert_endian_16(buffer->e_phnum,     is_lsb);
   hdr->e_shentsize  = convert_endian_16(buffer->e_shentsize, is_lsb);
   hdr->e_shnum      = convert_endian_16(buffer->e_shnum,     is_lsb);
   hdr->e_shstrndx   = convert_endian_16(buffer->e_shstrndx,  is_lsb);
}

static void Elf64_Parse_Ehdr(Elf_Ehdr *hdr, const Elf64_Ehdr *buffer){
   bool is_lsb = (buffer->e_ident[EI_DATA] == ELFDATA2LSB);

   hdr->e_version    = convert_endian_32(buffer->e_version,      is_lsb);
   hdr->e_type       = convert_endian_16(buffer->e_type,         is_lsb);
   hdr->e_machine    = convert_endian_16(buffer->e_machine,      is_lsb);
   hdr->e_entry      = convert_endian_64(buffer->e_entry,        is_lsb);
   hdr->e_phoff      = convert_endian_64(buffer->e_phoff,        is_lsb);
   hdr->e_shoff      = convert_endian_64(buffer->e_shoff,        is_lsb);
   hdr->e_flags      = convert_endian_32(buffer->e_flags,        is_lsb);
   hdr->e_ehsize     = convert_endian_16(buffer->e_ehsize,       is_lsb);
   hdr->e_phentsize  = convert_endian_16(buffer->e_phentsize,    is_lsb);
   hdr->e_phnum      = convert_endian_16(buffer->e_phnum,        is_lsb);
   hdr->e_shentsize  = convert_endian_16(buffer->e_shentsize,    is_lsb);
   hdr->e_shnum      = convert_endian_16(buffer->e_shnum,        is_lsb);
   hdr->e_shstrndx   = convert_endian_16(buffer->e_shstrndx,     is_lsb);
}

static void Elf_Parse_Ehdr(Elf_Ehdr *hdr, const Elf_Byte *buffer){
   // Parse header
   if(buffer[EI_CLASS] == ELFCLASS64){
      Elf64_Parse_Ehdr(hdr, (void*)buffer);
   }else{
      Elf32_Parse_Ehdr(hdr, (void*)buffer);
   }
}

static inline Elf_Half Elf_readHalf(const Elf_Byte *buffer, Elf_Word offset){
   Elf_Half val = *(Elf_Half*)(buffer +  offset);
   #if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
      if(buffer[EI_DATA] != ELFDATA2LSB) val = (val << 8) | (val >> 8);
   #elif __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__
      if(buffer[EI_DATA] != ELFDATA2MSB) val = (val << 8) | (val >> 8);
   #endif
   return val;
}

static inline Elf_Word Elf_readWord(const Elf_Byte *buffer, Elf_Word offset){
   Elf_Word val = *(Elf_Word*)(buffer +  offset);
   #if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
      if(buffer[EI_DATA] != ELFDATA2LSB) val =
         (val << 24) | ((val & 0x00FF0000) >> 8) |
         ((val & 0x0000FF00) << 8) | (val >> 24);
   #elif __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__
      if(buffer[EI_DATA] != ELFDATA2MSB) val =
         (val << 24) | ((val & 0x00FF0000) >> 8) |
         ((val & 0x0000FF00) << 8) | (val >> 24);
   #endif
   return val;
}

static inline Elf_Word Elf_readXword(const Elf_Byte *buffer, Elf_Word offset){
   Elf_Xword val = *(Elf_Xword*)(buffer +  offset);
   #if __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_LITTLE__
      if(buffer[EI_DATA] != ELFDATA2LSB) val =
         (val << 56) | ((val & 0x00FF000000000000) >> 8) |
         ((val & 0x0000FF0000000000) >> 24) |((val & 0x000000FF00000000) << 24)|
         ((val & 0x00000000FF000000) << 8) | (val >> 56);
   #elif __STDC_ENDIAN_NATIVE__ == __STDC_ENDIAN_BIG__
      if(buffer[EI_DATA] != ELFDATA2MSB) val =
         (val << 56) | ((val & 0x00FF000000000000) >> 8) |
         ((val & 0x0000FF0000000000) >> 24) |((val & 0x000000FF00000000) << 24)|
         ((val & 0x00000000FF000000) << 8) | (val >> 56);
   #endif
   return val;
}

static void Elf_Parse(Elf_Object *obj, Elf_Byte *buffer){}

#endif /* _ELF_GENERIC_H */
