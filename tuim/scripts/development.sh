export LD="clang"
export LDFLAGS="-fuse-ld=lld"
alias ld="${LD} ${LDFLAGS}"

export ASM="clang"
export ASMFLAGS="-c"
alias as="${ASM} ${ASMFLAGS}"

export CPP="clang -E -P"
export CPPFLAGS="-std=c23 "
alias cpp="${CPP} ${CPPFLAGS}"

export CC="clang"
export CFLAGS="-std=c23 -c"
alias cc="${CC} ${CFLAGS}"

make ( ) {
   [ -r "Make.sh" ] && { sh Make.sh $@; return $?; }
   echo "ERROR: No Make Script found."
   return 1
}
