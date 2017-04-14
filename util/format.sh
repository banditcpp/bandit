#!/bin/sh
# Reformat all tracked files (run this from bandit git root directory,
# not from util!)

DEFAULT_FORMAT_TOOL=clang-format-4.0
test -n "$FORMATTER" || FORMATTER="$(command -v "$DEFAULT_FORMAT_TOOL")"

if test -z "$FORMATTER"
then
	echo "Error: No '$DEFAULT_FORMAT_TOOL' executable found in PATH" >&2
	echo "You can define your clang-format path by setting the FORMATTER variable" >&2
	exit 2
fi

git ls-files '*.cpp' '*.h' | grep -v /external/ | xargs "$FORMATTER" -i
