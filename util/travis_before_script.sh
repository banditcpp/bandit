#!/bin/sh
# Script that is run by .travis.yml's before_script

mkdir -p build
cd build

if [ "$BUILD_TYPE" = codecov ]
then
	echo "Building in debug mode with coverage information"
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS_DEBUG="-g -O0 -fprofile-arcs -ftest-coverage" ..
else
	echo "Building in release mode"
	cmake -DCMAKE_BUILD_TYPE=Release ..
fi
exit
