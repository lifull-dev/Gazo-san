#!/bin/bash

GOOGLETEST_VER=1.10.0
curl -OLsS https://github.com/google/googletest/archive/release-"$GOOGLETEST_VER".tar.gz
tar -zxvf release-"$GOOGLETEST_VER".tar.gz
cd googletest-release-"$GOOGLETEST_VER" || exit
mkdir build
cd build || exit
cmake ..
make
cd ../../ || exit
mkdir -p tests/googletest
mv googletest-release-"$GOOGLETEST_VER"/build/lib/libg* tests/googletest/
mv googletest-release-"$GOOGLETEST_VER"/googletest/include tests/googletest/
rm -rf  googletest-release-"$GOOGLETEST_VER" release-"$GOOGLETEST_VER".tar.gz