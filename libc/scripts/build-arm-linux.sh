#!/usr/bin/env sh

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "lib" ] && { mkdir "lib" || exit $?; }

. "${TUIM_HOME}/share/dev-aarch32.sh"

as -o tmp/aarch32-linux.o src/aarch32-linux.asm

# stdio
cc -I ./include -o tmp/puts.o          src/stdio/puts.c

# string
cc -I ./include -o tmp/strlen.o        src/string/strlen.c

ld --shared -o lib/libc.so tmp/*.o
