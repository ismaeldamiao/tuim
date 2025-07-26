# Linker
export LD="ld.lld"
export LDFLAGS="--oformat=elf \
   --image-base=0x0 \
   -z separate-code \
   -z separate-loadable-segments \
   --warn-unresolved-symbols \
   --allow-shlib-undefined \
   --nostdlib \
   --hash-style=sysv \
   -z nognustack \
   -z norelro \
   --no-eh-frame-hdr \
   --dynamic-linker=/usr/bin/tuim \
   -L '${TUIM_HOME}/lib' "
alias ld="${LD} ${LDFLAGS}"

# Assembler
export ASM="clang -cc1as"
export ASMFLAGS="-triple armv7-unknown-unknown-eabi \
   -filetype obj \
   -mrelocation-model pic "
alias as="${ASM} ${ASMFLAGS}"

# LLVM compiler
export LLC="clang -c"
export LLFLAGS="--target=armv7-unknown-unknown-eabi -fPIC -O3"
alias llc="${LLC} ${LLFLAGS}"

# C preprocessor
export CPP="clang -cc1 -E -P"
export CPPFLAGS="-triple armv7-unknown-unknown-eabi"
alias cpp="${CPP} ${CPPFLAGS}"

# C compiler
export CC="clang -c"
export CFLAGS="--target=armv7-unknown-unknown-eabi \
   -Wno-error=unused-command-line-argument \
   -std=c23 \
   -pedantic \
   -ffreestanding \
   -flto \
   -fPIC \
   -nostdinc \
   -I '${TUIM_HOME}/include' \
   -O3 "
alias cc="${CC} ${CFLAGS}"
