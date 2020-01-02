# Common code to be run at the beginning of travis_*.sh scripts

if [ "$TRAVIS" != "true" ]
then
	echo "Error: These scripts should only be run by Travis CI." >&2
	exit 1
fi

if [ "$BUILD_TYPE" = "none" ]
then
	exit 0
fi
