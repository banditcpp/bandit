#!/bin/sh

if [ "$#" != 1 -o ! -x "$1" ]
then
	echo "Usage: $0 <path to bandit-specs executable>"
	exit 1
fi

DOCFILE_RELATIVE="docs/runningtests.md"
DOCFILE="$(git rev-parse --show-toplevel)/$DOCFILE_RELATIVE"
ACTUAL="$DOCFILE-actual-doc"
EXPECTED="$DOCFILE-expected-doc"
STARTLINE="$(git grep -n '^```bash$' "$DOCFILE" | cut -d : -f 2 | head -n 1)"
ENDLINE="$(git grep -n '^```$' "$DOCFILE" | cut -d : -f 2 | head -n 1)"
STARTLINE=$((STARTLINE + 1))
ENDLINE=$((ENDLINE - 1))

sed -n "${STARTLINE},${ENDLINE}p" "$DOCFILE" >"$ACTUAL"
echo "$ $1 --help" >"$EXPECTED"
"$1" --help --colorizer=off >>"$EXPECTED"

diff -u "$EXPECTED" "$ACTUAL"
ERRNO=$?

rm "$EXPECTED" "$ACTUAL"

if [ "$ERRNO" != 0 ]
then
	cat >&2 <<EOF

The output of

	$1 --help

changed (see diff above)."

Please update it in the

	$DOCFILE_RELATIVE

file.
EOF
fi

exit $ERRNO
