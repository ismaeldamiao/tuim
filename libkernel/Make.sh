#!/bin/sh

# ==============================================================================
# Make script for libkernel
# Last modified: September 11, 2025
# ==============================================================================

. ".make/include.sh"

LIBRARY="${SHARED_LIBRARY_PREFIX}kernel${SHARED_LIBRARY_SUFFIX}"

# =====================================
# Standard commands
# =====================================

build ( ) {

   SOURCES=$(cd "${SOURCE_DIR}"; echo *.*)

   mkdir -p "${BUILD_DIR}/lib" || return $?
   cp -R -L "${INCLUDE_DIR}" "${BUILD_DIR}/include" || return $?

   # Compile each source 
   local OBJECTS=""
   for SOURCE in ${SOURCES}; do
      local OBJECTS="${OBJECTS} ${BUILD_DIR}/tmp/${SOURCE%.*}.o"
      # call helper function
      compile "${SOURCE}" || return $?
   done

   # link all togheter
   ld --shared -o "${BUILD_DIR}/lib/${LIBRARY}" ${OBJECTS}
   return $?
}

install ( ) {
   if ! [ -d "${BUILD_DIR}/lib" ]; then
      build
   elif ! [ -r "${BUILD_DIR}/lib/${LIBRARY}" ]; then
      build
   fi

   [ -d "${TUIM_HOME}/lib" ] || mkdir -p "${TUIM_HOME}/lib"
   [ -d "${TUIM_HOME}/include/tuim" ] || mkdir -p "${TUIM_HOME}/include/tuim"

   cp "${BUILD_DIR}/lib/${LIBRARY}" "${PREFIX}/lib/${LIBRARY}"
   cp "${BUILD_DIR}/include/tuim/kernel.h" "${PREFIX}/include/tuim/kernel.h"
   cp "${BUILD_DIR}/include/tuim/arch.h" "${PREFIX}/include/tuim/arch.h"
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

[ "${DO_DEFAULT}" = "true" ] && { build || exit $?; }
exit 0
