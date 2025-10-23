# ==============================================================================
#                        Include file for Make scripts
# This file is used on The Tuim Projects to easily write Make Scripts.
#
# Last modified: September 11, 2025
# ==============================================================================

# =====================================
# Standard macros
# =====================================

SOURCE_DIR="src"
INCLUDE_DIR="include"
BUILD_DIR="build"
BUILD_TYPE="release"

STATIC_LIBRARY_PREFIX="lib"
SHARED_LIBRARY_PREFIX="lib"
STATIC_LIBRARY_SUFFIX=".a"
SHARED_LIBRARY_SUFFIX=".so"
EXECUTABLE_SUFFIX=".elf"

DESTDIR=""
PREFIX="${DESTDIR}/usr/local"
BINDIR="${PREFIX}/bin"
DATADIR="${PREFIX}/share"
MANDIR="${DATADIR}/man"

# =====================================
# Aliases
# =====================================

alias cpp="${CPP} ${CPPFLAGS}"
alias as="${ASM} ${ASMFLAGS}"
alias ld="${LD} ${LDFLAGS}"

# =====================================
# Helper functions
# =====================================

overhide_macro ( ) {
   case "${1%%=*}" in
      SOURCE_DIR) SOURCE_DIR="${1#*=}";;
      INCLUDE_DIR) INCLUDE_DIR="${1#*=}";;
      BUILD_DIR) BUILD_DIR="${1#*=}";;
      BUILD_TYPE) BUILD_TYPE="${1#*=}";;
      STATIC_LIBRARY_PREFIX) STATIC_LIBRARY_PREFIX="${1#*=}";;
      SHARED_LIBRARY_PREFIX) SHARED_LIBRARY_PREFIX="${1#*=}";;
      STATIC_LIBRARY_SUFFIX) STATIC_LIBRARY_SUFFIX="${1#*=}";;
      SHARED_LIBRARY_SUFFIX) SHARED_LIBRARY_SUFFIX="${1#*=}";;
      EXECUTABLE_SUFFIX) EXECUTABLE_SUFFIX="${1#*=}";;
      PREFIX) PREFIX="${1#*=}";;
      BINDIR) BINDIR="${1#*=}";;
      DATADIR) DATADIR="${1#*=}";;
      MANDIR) MANDIR="${1#*=}";;
      *) return 1;  # Return 1 for unrecognized variable
   esac
   return 0;  # Return 0 for successful assignment
}

# This function build a source file using implicity rules,
# based on the file extension
compile ( ) {
   local SOURCE="${1}"
   local OBJECT="${1%.*}.o"

   if [ "${OBJECT%%/*}" = "${OBJECT}" ]; then
      [ -d "${BUILD_DIR}/tmp" ] || mkdir -p "${BUILD_DIR}/tmp"
   else
      [-d "${BUILD_DIR}/tmp/${OBJECT%%/*}" ] || \
      mkdir -p "${BUILD_DIR}/tmp/${OBJECT%%/*}"
   fi

   # multi-step
   if [ "${SOURCE##*.}" = "S" ]; then
      # assembly (need to preprocess)
      local INPUT="${SOURCE_DIR}/${SOURCE}"
      local OUTPUT="${BUILD_DIR}/tmp/${SOURCE%%.S}.s"
      cpp -I "${INCLUDE_DIR}" -o "${OUTPUT}" "${INPUT}" || return $?

      local INPUT="${BUILD_DIR}/tmp/${SOURCE%%.S}.s"
      local OUTPUT="${BUILD_DIR}/tmp/${OBJECT}"
      as -o "${OUTPUT}" "${INPUT}" || return $?
      rm "${INPUT}"
      return 0
   fi

   # one step
   local INPUT="${SOURCE_DIR}/${SOURCE}"
   local OUTPUT="${BUILD_DIR}/tmp/${OBJECT}"
   if [ "${SOURCE##*.}" = "s" ]; then
      # assembly
      as -o "${OUTPUT}" "${INPUT}" || return $?
   fi
}
