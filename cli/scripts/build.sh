#!/usr/bin/env sh

[ -z "${CC}" ]     && CC=clang
[ ! -d "bin" ]  && { mkdir "bin" || exit $?; }

exec ${CC} -o bin/tuim -ltuim src/tuim.c
