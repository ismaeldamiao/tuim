#!/usr/bin/env sh

if [[ "${CC}" == "" ]]; then
   CC=clang
fi

if [[ "${PREFIX}" == "" ]]; then
   TUIM_HOME="/usr/lib/tuim"
else
   TUIM_HOME="${PREFIX}/lib/tuim"
fi

if [[ ! -d ".build" ]] || [[ ! -r ".build/tuim" ]]; then
   bash scripts/build.sh
fi

if [[ ! -d "${TUIM_HOME}" ]]; then
   mkdir -p "${TUIM_HOME}/{bin,include,lib}"
fi

####
# INSTALL
#####

if [[ "${PREFIX}" == "" ]]; then
   cp ".build/tuim" "/usr/bin/tuim"
else
   cp ".build/tuim" "${PREFIX}/bin/tuim"
fi

exit 0
