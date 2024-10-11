#ifndef ISA_ARM_H
#define ISA_ARM_H

/* ---
Register specifiers
--- */

#define R0  ((unsigned _BitInt(4))0x0)
#define R1  ((unsigned _BitInt(4))0x1)
#define R2  ((unsigned _BitInt(4))0x2)
#define R3  ((unsigned _BitInt(4))0x3)
#define R4  ((unsigned _BitInt(4))0x4)
#define R5  ((unsigned _BitInt(4))0x5)
#define R6  ((unsigned _BitInt(4))0x6)
#define R7  ((unsigned _BitInt(4))0x7)
#define R8  ((unsigned _BitInt(4))0x8)
#define R9  ((unsigned _BitInt(4))0x9)
#define R10 ((unsigned _BitInt(4))0xa)
#define R11 ((unsigned _BitInt(4))0xb)
#define R12 ((unsigned _BitInt(4))0xc)
#define SP  ((unsigned _BitInt(4))0xd)
#define LR  ((unsigned _BitInt(4))0xe)
#define PC  ((unsigned _BitInt(4))0xf)

/* The cast to unsigned _BitInt(32) ensures that the code do not use UB. */
#define ARM_INSTR(condiction, opcode, operands) (\
   ((((unsigned _BitInt(32))(condiction)) << 28) | \
   (((unsigned _BitInt(32))(opcode)) << 20)) | \
   ((unsigned _BitInt(32))(operands)) \
)

/* ---
Standard data-processing instructions
--- */

#define ADC
#define ADD
#define ADR
#define AND
#define BIC
#define CMN
#define CMP
#define EOR
#define MOV(Rd, op) _Generic( (op),\
   unsigned _BitInt(4): ARM_INSTR(0xE, 0x1A, ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
   unsigned _BitInt(8): ARM_INSTR(0xE, 0x3A, ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
   unsigned _BitInt(12):ARM_INSTR(0xE, 0x3A, ((unsigned _BitInt(32))(Rd) << 12) | (op)) \
)
#define NVM
#define ORN
#define ORR(Rd, Rn, op) _Generic( (op),\
   unsigned _BitInt(4): ARM_INSTR(0xE, 0x18, ((unsigned _BitInt(32))(Rn) << 16) | ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
   unsigned _BitInt(8): ARM_INSTR(0xE, 0x38, ((unsigned _BitInt(32))(Rn) << 16) | ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
   unsigned _BitInt(12):ARM_INSTR(0xE, 0x38, ((unsigned _BitInt(32))(Rn) << 16) | ((unsigned _BitInt(32))(Rd) << 12) | (op)) \
)
#define RSB
#define RSC
#define SBC
#define SUB
#define TEQ
#define TST

/* ---
Shift instructions
--- */

#define ASR
#define LSL
#define LSR
#define ROR
#define RRX

/* ---
Multiply instructions
--- */

#define MLA
#define MLS
#define MUL
#define SMLABB
#define SMLABT
#define SMLATB
#define SMLATT
#define SMLAD
#define SMLAL
#define SMLALBB
#define SMLALBT
#define SMLALTB
#define SMLALTT
#define SMLALD
#define SMLAWB
#define SMLAWT
#define SMLSD
#define SMLSLD
#define SMMLA
#define SMMLS
#define SMMUL
#define SMUAD
#define SMULBB
#define SMULBT
#define SMULTB
#define SMULTT
#define SMULL
#define SMULWB
#define SMULWT
#define SMUSD
#define UMAAL
#define UMLAL
#define UMULL

/* ---
Saturating instructions
--- */

#define SSAT
#define SSAT16
#define USAT
#define USAT16

/* ---
Saturating addition and subtraction instructions
--- */

#define QADD
#define QSUB
#define QDADD
#define QDSUB

/*
Status register access instructions
*/

/*
Load/store instructions
*/

//#define LDR(Rd, Rn) _Generic( (op),\
//   unsigned _BitInt(4): ARM_INSTR(0xE, 0x59, ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
//   unsigned _BitInt(8): ARM_INSTR(0xE, 0x59, ((unsigned _BitInt(32))(Rd) << 12) | (op)), \
//   unsigned _BitInt(12):ARM_INSTR(0xE, 0x59, ((unsigned _BitInt(32))(Rd) << 12) | (op)) \
//)

#define LDR(Rd, Rn, imm12) ARM_INSTR(0xE, 0x59, ((unsigned _BitInt(32))(Rd) << 12) | ((unsigned _BitInt(32))(Rn) << 16) | (unsigned _BitInt(32))(imm12))

/*
Load/store multiple instructions
*/

/*
Miscellaneous instructions
*/

/*
Exception-generating and exception-handling instruction
*/

/*
Coprocessor instructions
*/

/*
...
*/

#define STR(Rt, Rn) ARM_INSTR(0xE, 0x58, ((unsigned _BitInt(32))(Rn) << 16) | ((unsigned _BitInt(32))(Rt) << 12))
#define BX(op) ARM_INSTR(0xE, 0x12, 0xfff10 | (op))
#define SVC(op) ARM_INSTR(0xE, 0xf0, op)

#endif // ISA_ARM_H
