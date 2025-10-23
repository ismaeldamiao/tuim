#!/bin/sh

# ==============================================================================
# Make script for Tuim
# Last modified: October 02, 2025
# ==============================================================================

. ".make/include.sh"

# =====================================
# Standard commands
# =====================================

SOURCES="main.c tuim.c module_native/tuim_va.c"
OBJECTS=""


build ( ) {

   [ -d "${BUILD_DIR}/include" ] && rm -R "${BUILD_DIR}/include"
   cp -R -L "${INCLUDE_DIR}" "${BUILD_DIR}/include"

   for file in ${SOURCES}; do
      OBJECTS="${OBJECTS} ${BUILD_DIR}/tmp/${file%.*}.o"
      compile "${file}"
   done
   ld -e _start -o "${BUILD_DIR}/tuim" ${OBJECTS}

   return $?
}

install ( ) {

   # install headers
   [ -d "${TUIM_HOME}/include" ] || mkdir -p "${TUIM_HOME}/include"
   cp -Rf "${BUILD_DIR}/include/"* "${TUIM_HOME}/include"

   # install executable
   cp -f "${BUILD_DIR}/tuim" "${TUIM_HOME}/tuim"

   return 1
}

clean ( ) rm -Rf "${BUILD_DIR}"

# =====================================
# Argument handler
# =====================================

DO_DEFAULT=true
while [ $# != 0 ]; do

   # call helper function
   overhide_macro $1 && { shift; continue 1; }

   if [ "${1}" = "clean" ]; then
      DO_DEFAULT=false
      clean || exit $?
   elif [ "${1}" = "install" ]; then
      DO_DEFAULT=false
      install || exit $?
   fi
   shift
done

if [ "${BUILD_TYPE}" = "release" ]; then
   unalias cc
   unalias cpp
   alias cc="${CC} ${CFLAGS} -D NDEBUG"
   alias cpp="${CPP} ${CPPFLAGS} -D NDEBUG"
fi

[ "${DO_DEFAULT}" = "true" ] && { build || exit $?; }
exit 0
