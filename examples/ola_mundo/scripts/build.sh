#!/usr/bin/env sh

[ ! -d "tmp" ] && { mkdir "tmp" || exit $?; }
[ ! -d "bin" ] && { mkdir "bin" || exit $?; }

alias ld="${LD} ${LDFLAGS}"
alias cc="${CC} ${CFLAGS}"

cc -o tmp/main.o src/main.c || exit $?;
ld -e main -o bin/ola.elf -l c tmp/main.o || exit $?;
