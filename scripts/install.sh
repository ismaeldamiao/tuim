#/usr/bin/env sh

# -------------------------------------
#  Script to build the command line interface for Tuim and all reference
#  implementation of libraries.
#  * Part of Tuim project
#  * Last modified: February 27, 2025.
# -------------------------------------

if [ $# -lt 1 ]; then
  echo "Usage: ${0} <install_prefix>"
  exit 1
fi

export TUIM_HOME="${1}"
[ ! -d "${TUIM_HOME}" ] && { mkdir "${TUIM_HOME}" || exit $?; }

# Install shell utilities
[ ! -d "${TUIM_HOME}/share" ] && { mkdir "${TUIM_HOME}/share" || exit $?; }
cp -r skel/share/* "${TUIM_HOME}/share/" || exit $?
echo "Hint: Install '${TUIM_HOME}/share/dev-host.sh' as best fit your machine"

# Install the command line interface
if [ ! -e "interfaces/bin/tuim" ]; then
   echo "Run 'sh scripts/build.sh' first."
   exit 1
fi
[ ! -d "${TUIM_HOME}/bin" ] && { mkdir "${TUIM_HOME}/bin" || exit $?; }
cp "interfaces/bin/tuim" "${TUIM_HOME}/tuim" || exit $?

# Install reference implementation of libraries
[ ! -d "${TUIM_HOME}/lib" ] && { mkdir "${TUIM_HOME}/lib" || exit $?; }
cp "libraries/c/lib/libc.so" "${TUIM_HOME}/lib/libc.so" || exit $?

echo ""
echo "To complete the installation copy \"${TUIM_HOME}/tuim\" to the PATH and set"
echo "export TUIM_HOME=\"${TUIM_HOME}\""

exit 0
