#!/bin/sh

if [ $# -ne 1 ]
then
	echo "Usage: $0 <Travis CI phase name>"
	exit 1
fi
phase="$1"

if [ "$TRAVIS" != "true" ]
then
	echo "Error: This script should only be run by Travis CI." >&2
	exit 1
fi

case "$BUILD_TYPE" in
codecov|normal|check)
	;;
none)
	exit 0
	;;
*)
	echo "Error: BUILD_TYPE \"$BUILD_TYPE\" unknown." >&2
	exit 1
	;;
esac

case "$BUILD_TYPE:$phase" in
check:script)
	./util/check_header_guards.sh &&
	./util/check_selfsufficiency.sh
	;;
normal:before_script)
	mkdir -p build
	cd build

	echo "Building in release mode"
	cmake -DCMAKE_BUILD_TYPE=Release ..
	;;
codecov:before_script)
	mkdir -p build
	cd build

	echo "Building in debug mode with coverage information"
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_CXX_FLAGS_DEBUG="-g -O0 -fprofile-arcs -ftest-coverage" ..
	;;
normal:script|codecov:script)
	make -C build
	;;
codecov:after_success)
	echo "Collecting coverage information"
	lcov --directory . --capture --output-file coverage.info &&
	lcov --remove coverage.info '/usr/*' --output-file coverage.info &&
	lcov --list coverage.info || exit

	echo "Sending information to codecov.io"
	curl -s https://codecov.io/bash | bash
	;;
normal:after_success)
	../util/check_documentation_output.sh bin/bandit-specs
	;;
esac
