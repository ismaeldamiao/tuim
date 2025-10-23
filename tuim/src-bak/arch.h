/* *****************************************************************************
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
#ifndef TUIM_ARCH_H
#define TUIM_ARCH_H
/* ------------------------------------
   Generic implementation of <tuim/arch.h> header from The Tuim Interface.

   * Last modified: July 23, 2025.
------------------------------------ */

#define tuim_unknown          0x0000
#define tuim_riscv32          0x0001
#define tuim_riscv64          0x0002
#define tuim_arm              0x0003
#define tuim_aarch64          0x0004
#define tuim_i386             0x0005
#define tuim_amd64            0x0006

#if defined(__riscv) && __riscv == 32
   #define tuim_arch          tuim_riscv32
#elif defined(__riscv) && __riscv == 64
   #define tuim_arch          tuim_riscv64
#elif defined(__ARM_ARCH) && __ARM_ARCH <= 7
   #define tuim_arch          tuim_arm
#elif defined(__ARM_ARCH) && __ARM_ARCH >=8
   #define tuim_arch          tuim_aarch64
#elif defined(__i386__)
   #define tuim_arch          tuim_i386
#elif defined(__x86_64__)
   #define tuim_arch          tuim_amd64
#else
   #define tuim_arch          tuim_unknown
#endif

#endif /* TUIM_ARCH_H */
