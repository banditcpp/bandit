#!/bin/sh

ret=0

fail () {
	echo "$1"
	ret=1
}

for header in $(git ls-files 'bandit/*.h' | grep -v bandit/external/)
do
	guard=$(echo $header | tr '[a-z]/\.' '[A-Z]__')
	grep -q "^#ifndef $guard\$" $header || fail "Header guard for $header should be $guard"
done

exit $ret
