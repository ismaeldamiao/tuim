#!/bin/sh
# export ARCH
# export OSTYPE
# export TUIM_HOME
# sh -e tuim/scripts/build.sh

# compilers I know can build tuim ----------------------------------------------

if ! [ "$(command -v clang)" = "" ]; then
   export CC="clang"
   export CFLAGS="-Wall -Wextra -Wpedantic -O3 -c"
   export LD="clang"
   export LDFLAGS="-fuse-ld=lld"
elif ! [ "$(command -v gcc)" = "" ]; then
   export CC="gcc"
   export CFLAGS="-Wall -Wextra -O3 -c"
   export LD="gcc"
   export LDFLAGS=""
else
   export CC="cc"
   export CFLAGS="-c"
   export LD="cc"
   export LDFLAGS=""
fi

# OS I know can run tuim -------------------------------------------------------

cd tuim

if [ "${KERNEL}" = "linux" ]; then
   # Linux
   make ARCH=${ARCH} CFLAGS="${CFLAGS} -D _POSIX_C_SOURCE=200809L" ENTRY=_start tuim
   make ARCH=${ARCH} tuim/arch.h
elif [ "${KERNEL}" = "xnu" ]; then
   # XNU
   make CFLAGS="${CFLAGS} -D _POSIX_C_SOURCE=200809L" ENTRY=_main tuim
   make ARCH=${ARCH} tuim/arch.h
elif [ "${KERNEL}" = "windows_nt" ]; then
   # Windows NT
   make ENTRY=WinMain tuim
   make ARCH=${ARCH} tuim/arch.h
else
   exit 1
fi

cd ..

# install tuim -----------------------------------------------------------------

if [ "${KERNEL}" = "linux" ]; then
   # Linux
   mkdir -p "${TUIM_HOME}"
   mv tuim/bin/tuim "${TUIM_HOME}/tuim"
elif [ "${KERNEL}" = "xnu" ]; then
   # XNU
   mkdir -p "${TUIM_HOME}"
   mv tuim/bin/tuim "${TUIM_HOME}/tuim"
elif [ "${KERNEL}" = "windows_nt" ]; then
   # Windows NT
   mkdir -p "${TUIM_HOME}"
   mv tuim/bin/tuim.exe "${TUIM_HOME}/tuim.exe"
fi

mkdir -p "${TUIM_HOME}/include/tuim"
cp tuim/include/tuim.h      "${TUIM_HOME}/include/tuim.h"
mv tuim/include/tuim/arch.h "${TUIM_HOME}/include/tuim/arch.h"

if [ "${ARCH}" = "arm" ]; then
   cp "libcrt/share/dev-armv4t.sh" "${TUIM_HOME}/tuim-dev"
elif [ "${ARCH}" = "aarch64" ]; then
   cp "libcrt/share/dev-aarch64.sh" "${TUIM_HOME}/tuim-dev"
elif [ "${ARCH}" = "amd64" ]; then
   cp "libcrt/share/dev-x86_64.sh" "${TUIM_HOME}/tuim-dev"
else
   exit 1
fi

# build a default runtime for tuim ---------------------------------------------

. "${TUIM_HOME}/tuim-dev"

cd libkernel
make KERNEL=${KERNEL} libkernel.so
make KERNEL=${KERNEL} tuim/kernel.h
cd ..
mkdir -p "${TUIM_HOME}/lib" "${TUIM_HOME}/include/tuim"
mv libkernel/lib/libkernel.so "${TUIM_HOME}/lib/libkernel.so"
mv libkernel/include/tuim/kernel.h "${TUIM_HOME}/include/tuim/kernel.h"

cd libc
make libc.so
cd ..
mv libc/lib/libc.so "${TUIM_HOME}/lib/libc.so"
cp libc/include/* "${TUIM_HOME}/include"

exit 0
