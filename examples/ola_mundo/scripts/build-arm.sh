#!/usr/bin/env sh

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "bin" ] && { mkdir "bin" || exit $?; }

. "${TUIM_HOME}/share/env-arm.sh"

cc -o tmp/main.o src/main.c
ld -e main -o bin/main.elf -l c tmp/main.o
