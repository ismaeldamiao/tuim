#!/usr/bin/env sh

# ${1} OS kernel

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "lib" ] && { mkdir "lib" || exit $?; }

alias ld="${LD} ${LDFLAGS}"
alias as="${ASM} ${ASMFLAGS}"
alias llc="${LLC} ${LLFLAGS}"
alias cpp="${CPP} ${CPPFLAGS}"
alias cc="${CC} ${CFLAGS}"

if [ "${1}" = "linux" ]; then
   cpp -D __linux__=1 -o tmp/sys_write.asm       src/sys_write.S

   as -o tmp/sys_write.o                         tmp/sys_write.asm
fi

# stdio
cc -I ./include -emit-llvm -o tmp/puts.bc        src/stdio/puts.c

# string
cc -I ./include -emit-llvm -o tmp/strlen.bc      src/string/strlen.c

#llvm-link -o tmp/libc.bc tmp/*.bc
llc tmp/*.bc
ld --shared -o lib/libc.so tmp/*.o
