#!/bin/sh
# Script that is run by .travis.yml's after_success

if [ "$BUILD_TYPE" = codecov ]
then
	echo "Collecting coverage information"
	lcov --directory . --capture --output-file coverage.info &&
	lcov --remove coverage.info '/usr/*' --output-file coverage.info &&
	lcov --list coverage.info || exit

	echo "Sending information to codecov.io"
	curl -s https://codecov.io/bash | bash
else
	echo "Nothing to do"
fi
exit
