#!/usr/bin/env sh

[ -z "${CC}" ] && CC=clang
[ ! -d "lib" ] && { mkdir "lib" || exit $?; }
exec ${CC} -fPIC --shared -o lib/libtuim.so src/libtuim.c
