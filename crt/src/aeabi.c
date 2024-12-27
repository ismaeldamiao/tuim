/* *****************************************************************************
   MIT License

   Copyright (c) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>

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

/* *****************************************************************************
   Run-time ABI for the Arm Architecture.
   * Compile this with a ARM ABI compliat compiler.
   $ cc 
   Last modified: December 12, 2024.
***************************************************************************** */

//#include "__aeabi_dadd.c"
//#include "__aeabi_ddiv.c"
//#include "__aeabi_dmul.c"
//#include "__aeabi_drsub.c"
//#include "__aeabi_dsub.c"

//#include "__aeabi_cdcmpeq.c"
//#include "__aeabi_cdcmple.c"
//#include "__aeabi_cdrcmple.c"

//#include "__aeabi_dcmpeq.c"
//#include "__aeabi_dcmplt.c"
//#include "__aeabi_dcmple.c"
//#include "__aeabi_dcmpge.c"
//#include "__aeabi_dcmpgt.c"
//#include "__aeabi_dcmpun.c"

//#include "__aeabi_fadd.c"
//#include "__aeabi_fdiv.c"
//#include "__aeabi_fmul.c"
//#include "__aeabi_frsub.c"
//#include "__aeabi_fsub.c"

//#include "__aeabi_cfcmpeq.c"
//#include "__aeabi_cfcmple.c"
//#include "__aeabi_cfrcmple.c"
//#include "__aeabi_fcmpeq.c"
//#include "__aeabi_fcmplt.c"
//#include "__aeabi_fcmple.c"
//#include "__aeabi_fcmpge.c"
//#include "__aeabi_fcmpgt.c"
//#include "__aeabi_fcmpun.c"

//#include "__aeabi_d2iz.c"
//#include "__aeabi_d2uiz.c"
//#include "__aeabi_d2lz.c"
//#include "__aeabi_d2ulz.c"
//#include "__aeabi_f2iz.c"
//#include "__aeabi_f2uiz.c"
//#include "__aeabi_f2lz.c"
//#include "__aeabi_f2ulz.c"

//#include "__aeabi_d2f.c"
//#include "__aeabi_f2d.c"
//#include "__aeabi_h2f.c"
//#include "__aeabi_h2f_alt.c"
//#include "__aeabi_f2h.c"
//#include "__aeabi_f2h_alt.c"
//#include "__aeabi_d2h.c"
//#include "__aeabi_d2h_alt.c"

//#include "__aeabi_i2d.c"
//#include "__aeabi_ui2d.c"
//#include "__aeabi_l2d.c"
//#include "__aeabi_ul2d.c"
//#include "__aeabi_i2f.c"
//#include "__aeabi_ui2f.c"
//#include "__aeabi_l2f.c"
//#include "__aeabi_ul2f.c"

/* *****************************************************************************
The long long helper functions
***************************************************************************** */

//#include "__aeabi_lmul.c"
//#include "__aeabi_ldivmod.c"
//#include "__aeabi_uldivmod.c"
//#include "__aeabi_llsl.c"
//#include "__aeabi_llsr.c"
//#include "__aeabi_lasr.c"
//#include "__aeabi_lcmp.c"
//#include "__aeabi_ulcmp.c"

/* ---
5.3   Other C and assembly language helper functions
--- */

/* 5.3.1   Integer (32/32 -> 32) division functions */

//#include "idiv.c"

/* 5.3.2   Division by zero */
#include "div0.c"

/* 5.3.3   Unaligned memory access */
#include "rw.c"

/* 5.3.4   Memory copying, clearing, and setting */
#include "mem.c"

/* 5.3.5   Thread-local storage (new in v2.01) */
#include "thread_local.c"
/* *****************************************************************************
   "Principium sapientiae timor Domini, et scientia sanctorum prudentia."
***************************************************************************** */
