#!/usr/bin/env sh

if [[ "${CC}" == "" ]]; then
   CC=clang
fi

if [[ "${PREFIX}" == "" ]]; then
   TUIM_HOME="/usr/lib/tuim"
else
   TUIM_HOME="${PREFIX}/lib/tuim"
fi

####
# BUILD
#####

if [[ ! -d ".build" ]]; then
   mkdir ".build"
fi

${CC} -c -fPIC \
   -DTUIM_HOME="\"${TUIM_HOME}\"" \
   -o .build/libtuim.o \
   src-loader/libtuim.c

${CC} -fPIE \
   -I src-loader/include \
   -o .build/tuim \
   src-cli/tuim.c .build/libtuim.o

exit 0
