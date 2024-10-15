#!/usr/bin/env sh

[ "${PREFIX}" == "" ] && export PREFIX="/usr"

TUIM_HOME="${PREFIX}/lib/tuim"

[ ! -d ".build" ] || [ ! -r ".build/tuim" ] && {
   bash scripts/build.sh || exit $?
}

[ ! -d "${TUIM_HOME}" ] && {
   mkdir -p "${TUIM_HOME}"/bin || exit $?
   mkdir -p "${TUIM_HOME}"/include || exit $?
   mkdir -p "${TUIM_HOME}"/lib || exit $?
}

####
# INSTALL
#####

if [ "${PREFIX}" == "" ]; then
   cp ".build/tuim" "/usr/bin/tuim" || exit $?
else
   cp ".build/tuim" "${PREFIX}/bin/tuim" || exit $?
fi

exit 0
