#!/bin/sh
# Script that is run by .travis.yml's script step

. util/travis_common.sh || exit 1

case "$BUILD_TYPE" in
check)
	./util/check_header_guards.sh &&
	./util/check_selfsufficiency.sh
	;;
coverity)
	export COVERITY_SCAN_PROJECT_NAME=banditcpp/bandit
	export COVERITY_SCAN_NOTIFICATION_EMAIL=nope
	export COVERITY_SCAN_BRANCH_PATTERN=coverity-scan
	export COVERITY_SCAN_BUILD_COMMAND="make -C build"
	curl -s "https://scan.coverity.com/scripts/travisci_build_coverity_scan.sh" | bash
	;;
normal|codecov)
	make -C build
	;;
*)
	echo "BUILD_TYPE \"$BUILD_TYPE\" not set or unknown." >&2
	exit 1
	;;
esac
