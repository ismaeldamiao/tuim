#!/bin/sh
# sh -e docs/assets/linux.sh

# compilers I know can build tuim ----------------------------------------------

if ! [ "$(command -v clang)" = "" ]; then
   export CPP="clang"
   export CPPFLAGS="-E"
   export CC="clang"
   export CFLAGS="-Wall -Wextra -Wpedantic -O3 -c"
   export LD="clang"
   export LDFLAGS="-fuse-ld=lld"
elif ! [ "$(command -v gcc)" = "" ]; then
   export CC="gcc"
   export CFLAGS="-Wall -Wextra -O3 -c"
   export LD="ld"
   export LDFLAGS=""
else
   export CC="cc"
   export CFLAGS="-c"
   export LD="cc"
   export LDFLAGS=""
fi

# prepare destinatin -----------------------------------------------------------

mkdir -p "${TUIM_HOME}"
mkdir -p "${TUIM_HOME}/bin"
mkdir -p "${TUIM_HOME}/lib"
mkdir -p "${TUIM_HOME}/include/tuim"
mkdir -p "${TUIM_HOME}/etc/profile.d"

# build and install tuim -------------------------------------------------------


cd libtuim
make libtuim.o
cd ..


cd tuim
cp -r ../libtuim/lib ./
cp -r ../libtuim/include ./
mkdir -p bin
${CC} -I include -o bin/tuim src/main.c lib/libtuim.o
cp bin/tuim "${TUIM_HOME}/tuim"
ln -sf "${TUIM_HOME}/tuim" "${PREFIX}/bin/tuim"
cd ..


# build a useful environment ---------------------------------------------------


cp "tuim/src/kernel-linux.h" "${TUIM_HOME}/include/tuim/kernel.h"
if [ -r "arch.h" ]; then
   mv "arch.h" "${TUIM_HOME}/include/tuim/"
fi
if [ -r "environment.sh" ]; then
   mv "environment.sh" "${TUIM_HOME}/etc/profile.d/"
fi

. "${TUIM_HOME}/etc/profile.d/environment.sh"


cd libkernel
make libkernel.so
cp lib/libkernel.so "${TUIM_HOME}/lib/libkernel.so"
cd ..


cd libc
make libc.so
cp lib/libc.so "${TUIM_HOME}/lib/libc.so"
cd ..


cd libtuim
#make libtuim.so
cp include/tuim.h "${TUIM_HOME}/include/tuim.h"
#cp lib/libtuim.so "${TUIM_HOME}/lib/libtuim.so"
cd ..
