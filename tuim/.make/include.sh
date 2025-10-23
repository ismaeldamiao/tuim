# ==============================================================================
#                        Include file for Make Scripts
# This file is used by The Tuim Projects to easily write Make Scripts.
#
# Last modified: October 03, 2025
# ==============================================================================

SCRIPT_DIR="$(echo "${PWD}")"

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

alias ld="${LD} ${LDFLAGS}"
alias cpp="${CPP} ${CPPFLAGS}"
alias as="${ASM} ${ASMFLAGS}"
alias cc="${CC} ${CFLAGS}"

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

   [ -r "${SCRIPT_DIR}/${SOURCE_DIR}/${SOURCE}" ] || {
      echo "make: cannot build ${SOURCE}: No such file" >&2
      return 1
   }

   local TMP="${BUILD_DIR}/tmp/${1}"
   local TMP="${TMP%/*}"
   [ -d "${TMP}" ] || { mkdir -p "${TMP}" || return $?; }

   # documents
   if [ "${SOURCE##*.}" = "tex" ]; then
      # --- LaTeX ---
      local INPUT="${SCRIPT_DIR}/${SOURCE_DIR}/${SOURCE}"
      local OUTPUT="${BUILD_DIR}/tmp/${SOURCE%.tex}.pdf"

      cd "${OUTPUT%/*}"
      TEXINPUTS="${INPUT%/*}//:" \
      BIBINPUTS="${INPUT%/*}:" \
      latexmk \
         -pdflatex \
         -silent -quiet \
         -f -g \
         -logfilewarninglist \
         "${INPUT}"
      cd "${SCRIPT_DIR}"

      [ -r "${OUTPUT}" ] || return 1
      local TMP="${BUILD_DIR}/${SOURCE%.tex}.pdf"
      [ -d "${TMP%/*}" ] || { mkdir -p "${TMP%/*}" || return $?; }
      mv "${OUTPUT}" "${TMP}"

      return $?
   fi

   # multi-step
   local OBJECT="${1%.*}.o"
   if [ "${SOURCE##*.}" = "S" ]; then
      # --- assembly ---
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
      # --- assembly ----
      as -o "${OUTPUT}" "${INPUT}" || return $?
   elif [ "${SOURCE##*.}" = "c" ]; then
      # --- C Programing Language ----
      if [ -d "${INCLUDE_DIR}" ]; then
         cc -I "${INCLUDE_DIR}" -o "${OUTPUT}" "${INPUT}" || return $?
      else
         cc -o "${OUTPUT}" "${INPUT}" || return $?
      fi
   else
      echo "make: there is no implicit rule to build ${SOURCE}" >&2
   fi
}

#basename ( ) { B="${1##*/}"; B="${B%$2}"; echo "${B}"; }
#dirname ( ) { local D="${1%/*}"; [ "${D}" = "${1}" ] && echo . || echo "${D}"; }
