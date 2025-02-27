#/usr/bin/env sh

# -------------------------------------
#  Script to test the current installation of Tuim.
#  * Part of Tuim project
#  * Last modified: February 27, 2025.
# -------------------------------------

if [ $# -lt 1 ]; then
  echo "Usage: ${0} <arquitecture>"
  exit 1
fi

cd examples/ola_mundo
tuim run bin/ola-${1}.elf || echo "Warning: tuim was exited the error code $?"
cd ..