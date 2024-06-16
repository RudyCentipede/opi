#!/bin/bash



"./clean.sh"
echo "~~~COVERAGE BUILD~~~"
"./build_gcov.sh"
cd ./func_tests/scripts/ || exit 1
./func_tests.sh
cd ../../ || exit 1

"./collect_coverage.sh"

