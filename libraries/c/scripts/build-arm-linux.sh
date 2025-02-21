#!/usr/bin/env sh

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "lib" ] && { mkdir "lib" || exit $?; }

. "${TUIM_HOME}/share/env-arm.sh"

as -o tmp/puts.o        src/linux/aarch32/puts.asm
cc -o tmp/strlen.o      src/generic/string/strlen.c

ld --shared -o lib/libc.so tmp/*.o
