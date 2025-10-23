#!/bin/sh
set -e
# ==============================================================================
# Make script for LaTeX documents
# Last modified: September 25, 2025
# ==============================================================================

. ".make/include.sh"

# =====================================
# Commands
# =====================================

build ( ) {
   compile "${1}/main.tex"
   return $?
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
   elif [ "${1}" = "std_dev" ]; then
      DO_DEFAULT=false
      build std_dev || exit $?
   elif [ "${1}" = "std_abi" ]; then
      DO_DEFAULT=false
      build std_abi || exit $?
   elif [ "${1}" = "r_tuim" ]; then
      DO_DEFAULT=false
      build r_tuim || exit $?
   fi
   shift
done

[ "${DO_DEFAULT}" = "true" ] && { build all || exit $?; }
exit 0
