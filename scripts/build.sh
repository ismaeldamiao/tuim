#!/usr/bin/env sh

[ "${CC}" == "" ]     && export CC=clang
[ "${PREFIX}" == "" ] && export PREFIX="/usr"

TUIM_HOME="${PREFIX}/lib/tuim"

####
# BUILD
#####

[ ! -d ".build" ] && {
   mkdir ".build" || exit $?
}

${CC} -c -fPIC \
   -DTUIM_HOME="\"${TUIM_HOME}\"" \
   -o .build/libtuim.o \
   src-loader/libtuim.c || exit $?

${CC} -fPIE \
   -I src-loader/include \
   -o .build/tuim \
   src-cli/tuim.c .build/libtuim.o || exit $?

exit 0