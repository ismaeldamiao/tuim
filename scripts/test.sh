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

echo ""
echo "--- Test 1: examples/ola_mundo ---"
tuim run examples/ola_mundo/bin/ola-${1}.elf || exit $?

exit 0
