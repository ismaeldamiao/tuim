#!/usr/bin/env sh

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "bin" ] && { mkdir "bin" || exit $?; }

# 32-bits

. "${TUIM_HOME}/share/dev-riscv32.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-riscv32.elf -L ../../libraries/c/lib/riscv32 -l c tmp/main.o

. "${TUIM_HOME}/share/dev-armv7.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-armv7.elf -L ../../libraries/c/lib/armv7 -l c tmp/main.o

. "${TUIM_HOME}/share/dev-i386.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-i386.elf -L ../../libraries/c/lib/i386 -l c tmp/main.o

# 64-bits

. "${TUIM_HOME}/share/dev-riscv64.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-riscv64.elf -L ../../libraries/c/lib/riscv64 -l c tmp/main.o

. "${TUIM_HOME}/share/dev-aarch64.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-aarch64.elf -L ../../libraries/c/lib/aarch64 -l c tmp/main.o

. "${TUIM_HOME}/share/dev-x86_64.sh"
cc -o tmp/main.o src/main.c
ld -e main -o bin/ola-x86_64.elf -L ../../libraries/c/lib/x86_64 -l c tmp/main.o
