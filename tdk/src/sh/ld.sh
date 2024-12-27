#!/usr/bin/env sh
: '
   Copyright (c) 2024 I.F.F. dos SANTOS <ismaellxd@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy 
   of this software and associated documentation files (the “Software”), to 
   deal in the Software without restriction, including without limitation the 
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
   sell copies of the Software, and to permit persons to whom the Software is 
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in 
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
   IN THE SOFTWARE.
'

# ###
# Get the default target
# ###
if echo "$(uname -m)" | grep -q "armv7"; then
   DEFAULT_TARGET=armv7
elif echo "$(uname -m)" | grep -q "arm"; then
   DEFAULT_TARGET=arm
else
   DEFAULT_TARGET="$(uname -m)"
fi

TDK_VERSION="0"
VERSION="0.1"
VERSION_STR="TDK version ${TDK_VERSION}
Copyright (C) 2024 I.F.F. dos Santos <ismaellxd@gmail.com>
|-- version ${VERSION}
\`-- configured for the target  ${DEFAULT_TARGET}"

# ###
# Parse arguments
# ###
IS_SHARED=false
ENTRY="main"
LIBS=""
OUTPUT=""
FILE=""
TARGET="${DEFAULT_TARGET}"

arg0=""
for arg in ${@}; do
   if [ "${arg:0:1}" = "-" ]; then
      if [ "${arg}" = "--version" ]; then
         echo "${VERSION_STR}"
         exit 0
      elif [ "${arg}" = "--shared" ]; then
         IS_SHARED=true
      fi
   else
      if ! [ "${arg0:0:1}" = "-" ]; then
         FILE="${FILE} ${arg}"
      elif [ "${arg0:0:2}" = "--" ]; then
         FILE="${FILE} ${arg}"
      elif [ "${arg0:0:2}" = "-l" ]; then
         FILE="${FILE} ${arg}"
      fi
   fi
   arg0="${arg}"
done

while getopts 'e:l:o:' arg; do
   if [ "${arg}" = "e" ]; then
      ENTRY="${OPTARG}"
   elif [ "${arg}" = "l" ]; then
      LIBS="${LIBS} ${OPTARG}"
   elif [ "${arg}" = "o" ]; then
      OUTPUT="${OPTARG}"
   fi
done

# ###
# Wrap to LLVM LLD
# ###
LLD="clang "
#LLD="clang -v"
if echo "${TARGET}" | grep -q "arm"; then
   LLD="${LLD} --target=${TARGET}-eabi "
else
   LLD="${LLD} --target=${TARGET}-elf "
fi

LLD="${LLD} -fuse-ld=lld -nostdlib -L ${TUIM_HOME}/lib"
LLD="${LLD} -Wl,--image-base=0x0,-z,separate-code,-z,separate-loadable-segments "
#LLD="${LLD} -Wl,--unresolved-symbols=ignore-all "
# Disable non standard ELF features
LLD="${LLD} -Wl,--hash-style=sysv " # do not use SHT_GNU_HASH
LLD="${LLD} -Wl,-z,nognustack " # do not use PT_GNU_STACK
LLD="${LLD} -Wl,-z,norelro " # do not use PT_GNU_RELRO
LLD="${LLD} -Wl,--no-eh-frame-hdr " # do not use PT_GNU_EH_FRAME

if [ "${IS_SHARED}" = false ]; then
   LLD="${LLD} -e ${ENTRY}"
   LLD="${LLD} -Wl,-dynamic-linker,/usr/bin/tuim "
else
   LLD="${LLD} -Wl,--shared,-Bsymbolic "
fi

if ! [ -z "${OUTPUT}" ]; then
   LLD="${LLD} -o ${OUTPUT}"
fi

LLD="${LLD} ${FILE}"

if ! [ -z "${LIBS}" ]; then
   for lib in ${LIBS}; do
      LLD="${LLD} -l${lib}"
   done
fi

#echo "${LLD}"
exec ${LLD}
