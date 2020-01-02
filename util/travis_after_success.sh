#!/bin/sh
# Script that is run by .travis.yml's after_success step

. util/travis_common.sh || exit 1

case "$BUILD_TYPE" in
codecov)
	echo "Collecting coverage information"
	lcov --directory . --capture --output-file coverage.info &&
	lcov --remove coverage.info '/usr/*' --output-file coverage.info &&
	lcov --list coverage.info || exit

	echo "Sending information to codecov.io"
	curl -s https://codecov.io/bash | bash
	;;
check)
	;;
normal)
	../util/check_documentation_output.sh bin/bandit-specs
	;;
*)
	echo "BUILD_TYPE \"$BUILD_TYPE\" not set or unknown." >&2
	exit 1
	;;
esac
