#!/bin/sh
# Script that is run by .travis.yml's script step

case "$BUILD_TYPE" in
check)
	./util/check_header_guards.sh &&
	./util/check_selfsufficiency.sh
	;;
normal|codecov)
	make -C build
	;;
*)
	echo "BUILD_TYPE \"$BUILD_TYPE\" not set or unknown." >&2
	exit 1
	;;
esac
