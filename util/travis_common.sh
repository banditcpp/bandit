# Common code to be run at the beginning of travis_*.sh scripts

if [ "$TRAVIS" != "true" ]
then
	echo "Error: These scripts should only be run by Travis CI." >&2
	exit 1
fi

if [ "$TRAVIS_BRANCH" = "coverity-scan" ]
then
	if [ "$BUILD_TYPE" != "coverity" ]
	then
		echo "This is the coverity-scan branch. Doing nothing."
		exit 0
	fi
elif [ "$BUILD_TYPE" = "coverity" ]
then
	echo "This is not the coverity-scan branch. Doing nothing."
	exit 0
fi
