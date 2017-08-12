#!/bin/sh

FILES="$*"
test -n "$FILES" || FILES="$(git ls-files *.h)"

ret=0
test -n "$CXX" || CXX=c++

"$CXX" -std=c++11 -I. -Wall -Wextra $FILES || ret=1
find -name '*.h.gch' -exec 'rm' '{}' ';'

exit $ret
