#!/usr/bin/env sh

[ -z "${CC}" ]     && CC=clang
[ -z "${PREFIX}" ] && PREFIX="/usr"

####
# BUILD
#####

[ ! -d ".build" ] && {
   mkdir ".build" || exit $?
}

echo "Building the C Standard Library..."

sh scripts/cc \
   -D __linux__ \
   -nostdinc -I src/include \
   -o .build/libc.o \
   src/libc.c || exit $?

if echo "$(uname -m)" | grep -q "arm"; then
   sh scripts/tuim-ld \
      --shared \
      -o .build/libc.so -laeabi \
      .build/libc.o || exit $?
else
   sh scripts/tuim-ld \
      --shared \
      -o .build/libc.so \
      .build/libc.o || exit $?
fi

exit 0
