#/usr/bin/env sh

export TUIM_HOME="${1}/tuim"
alias cc=${CC} ${CFLAGS}

if [ ! -d "${TUIM_HOME}" ]; then
   echo "Creating the TUIM_HOME"
   mkdir "${TUIM_HOME}"
   mkdir "${TUIM_HOME}/bin"
   mkdir "${TUIM_HOME}/lib"
   cp skel/share "${TUIM_HOME}/share"
   cp "skel/share/dev-${2}.sh" "${TUIM_HOME}/share/dev-host.sh"
fi


echo "Building the Interpreter's command line interface and install it"
cd interfaces
mkdir bin
cc -o "${TUIM_HOME}/tuim" -D _POSIX_C_SOURCE=200809L src/main.c
ln -sf "${TUIM_HOME}/tuim" "${HOME}/bin/tuim" # "install" the executable
cd ..


echo "Building the C Standard Library"
cd libraries/c
. "${TUIM_HOME}/share/dev-host.sh"
sh scripts/build.sh ${2} ${3}
cp lib/libc.so "${TUIM_HOME}/lib/libc.so"
cd ..


echo "Executing examples"
cd examples/ola_mundo
tuim run bin/ola-${2}.elf
