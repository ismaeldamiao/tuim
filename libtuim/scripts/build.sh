#!/usr/bin/env sh

[ -z "${CC}" ] && CC=clang

####
# BUILD
#####

[ ! -d "lib" ] && {
   mkdir "lib" || exit $?
}
[ ! -d "include" ] && {
   mkdir "include" || exit $?
}

cp src/include/tuim.h include/tuim.h

#${CC} -Wall -Wextra -Wpedantic -Werror \
${CC} $@ \
   --shared -fPIC \
   -o lib/libtuim.so \
   src/libtuim.c || exit $?

exit 0
