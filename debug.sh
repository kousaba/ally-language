#!/bin/bash
set -u

./build.sh

echo "Normal running ally compiler..."
echo ""
echo "----------Result----------"
exit_code=0
./build/ally-lang || exit_code=$?

echo "Exit Code: $exit_code"

if [ "$exit_code" -eq 0 ]; then
  echo "Successfully compleated."
  exit 0
elif [ "$exit_code" -eq 139 ] || [ "$exit_code" -eq 134 ]; then
  [ "$exit_code" -eq 139 ] && echo "Detected SIGSEGV"
  [ "$exit_code" -eq 134 ] && echo "Detected ABORT (ASan memory conflict?)"
  echo "Starting Debug Compiling with ASan..."
  ./debug_build.sh
  echo "--- Running with Debug Config ---"
  setarch `uname -m` -R gdb -batch -q -nh \
    -ex "set disable-randomization on" \
    -ex "run" \
    -ex "echo \n--- [1] Source Context (Nearby Code) ---\n" \
    -ex "list" \
    -ex "echo \n--- [2] Register State (CPU) ---\n" \
    -ex "info registers" \
    -ex "echo \n--- [3] Variable State (In function) ---\n" \
    -ex "info locals" \
    -ex "echo \n--- [4] Detailed Backtrace with Variables ---\n" \
    -ex "bt full" \
    -ex "quit" \
    ./build/ally-lang
else 
  echo "Terminated with unexpected exit code: $exit_code"
fi
