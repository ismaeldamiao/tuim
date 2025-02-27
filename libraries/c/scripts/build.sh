#!/usr/bin/env sh

# ${1} architecture
# ${2} OS

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "lib" ] && { mkdir "lib" || exit $?; }

alias as="${ASM} ${ASMFLAGS}"
alias cc="${CC} ${CFLAGS}"
alias ld="${LD} ${LDFLAGS}"

if [ "${2}" = "linux" ]; then
   as -o tmp/systemcall.o              src/${1}-linux.asm
fi

# stdio
cc -I ./include -o tmp/puts.o          src/stdio/puts.c

# string
cc -I ./include -o tmp/strlen.o        src/string/strlen.c

ld --shared -o lib/libc.so tmp/*.o
