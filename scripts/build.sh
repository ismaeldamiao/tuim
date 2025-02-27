#/usr/bin/env sh

# -------------------------------------
#  Script to build the command line interface for Tuim and all reference
#  implementation of libraries.
#  * Part of Tuim project
#  * Last modified: February 27, 2025.
# -------------------------------------

if [ $# -lt 2 ]; then
  echo "Usage: ${0} <arquiteture> <kernel>"
  exit 1
fi

if [ -z "${CC}" ]; then
   echo "Warnning: Unknown host compiler, defaulting to clang."
   export CC=clang
fi

alias cc=${CC} ${CFLAGS}

echo "Building the Interpreter's command line interface and install it"
cd interfaces
[ ! -d "bin" ] && { mkdir "bin" || exit $?; }
cc -o "bin/tuim" -D _POSIX_C_SOURCE=200809L src/main.c || exit $?
cd ../


echo "Building the C Standard Library"
cd libraries/c
. "../../skel/share/dev-${1}.sh"
sh scripts/build.sh ${1} ${2} || exit $?
cd ../../

exit 0
