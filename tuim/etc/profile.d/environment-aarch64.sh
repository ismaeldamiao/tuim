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
export ASMFLAGS="-triple aarch64-unknown-unknown-elf \
   -filetype obj \
   -mrelocation-model pic "
alias as="${ASM} ${ASMFLAGS}"

# C preprocessor
export CPP="clang -cc1 -E -P -I '${TUIM_HOME}/include'"
export CPPFLAGS="-triple aarch64-unknown-unknown-elf"
alias cpp="${CPP} ${CPPFLAGS}"

# C compiler
export CC="clang -cc1"
export CFLAGS="-triple aarch64-unknown-unknown-elf \
   -Wno-error=unused-command-line-argument \
   -std=c23 \
   -pedantic \
   -ffreestanding \
   -pic-level 2 \
   -nostdsysteminc \
   -I '${TUIM_HOME}/include' \
   -emit-obj \
   -O3 "
alias cc="${CC} ${CFLAGS}"
