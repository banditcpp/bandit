#!/bin/sh
# Script that is run by .travis.yml's before_script step

. util/travis_common.sh || exit 1

mkdir -p build
cd build

case "$BUILD_TYPE" in
codecov)
	echo "Building in debug mode with coverage information"
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS_DEBUG="-g -O0 -fprofile-arcs -ftest-coverage" ..
	;;
normal|coverity)
	echo "Building in release mode"
	cmake -DCMAKE_BUILD_TYPE=Release ..
	;;
check)
	;;
*)
	echo "BUILD_TYPE \"$BUILD_TYPE\" not set or unknown." >&2
	exit 1
	;;
esac
