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
export ASMFLAGS="-triple armv4t-unknown-unknown-eabi \
   -filetype obj \
   -mrelocation-model pic "
alias as="${ASM} ${ASMFLAGS}"

# C preprocessor
export CPP="clang -cc1 -E -P -I '${TUIM_HOME}/include'"
export CPPFLAGS="-triple armv4t-unknown-unknown-eabi"
alias cpp="${CPP} ${CPPFLAGS}"

# C compiler
export CC="clang -cc1"
export CFLAGS="-triple armv4t-unknown-unknown-eabi \
   -Wno-error=unused-command-line-argument \
   -std=c23 \
   -pedantic \
   -ffreestanding \
   -flto \
   -pic-level 2 \
   -nostdsysteminc \
   -I '${TUIM_HOME}/include' \
   -emit-obj \
   -O3 "
alias cc="${CC} ${CFLAGS}"
