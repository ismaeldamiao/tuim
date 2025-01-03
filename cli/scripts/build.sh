#!/usr/bin/env sh

[ -z "${CC}" ]     && CC=clang
[ -z "${PREFIX}" ] && PREFIX="/usr"

TUIM_HOME="${PREFIX}/lib/tuim"

####
# BUILD
#####

[ ! -d ".build" ] && {
   mkdir ".build" || exit $?
}

#${CC} -Wall -Wextra -Wpedantic -Werror \
${CC} \
   -c -fPIC \
   -DTUIM_HOME="\"${TUIM_HOME}\"" \
   -o .build/libtuim.o \
   src-loader/libtuim.c || exit $?

#${CC} -Wall -Wextra -Wpedantic -Werror \
${CC} \
   -fPIE \
   -I src-loader/include \
   -o .build/tuim \
   src-cli/tuim.c .build/libtuim.o || exit $?

exit 0
