#!/bin/sh

git diff-index --exit-code --numstat HEAD --ignore-submodules || exit

cleanup () {
	git checkout -
	exit
}

git checkout headers-only &&
git merge -s recursive -X subtree=bandit main || bash
git merge --abort 2>/dev/null && exit 1
commit="$(git show | grep ^Merge | cut -d ' ' -f 3)"
test -z "$commit" && cleanup # finished if merge had nothing to do

# update docs
git show main:docs/LICENSE.txt >LICENSE.txt
git show main:docs/index.md >README.md
git diff-index --exit-code --stat HEAD --ignore-submodules ||
	git commit -C HEAD -a --amend

# update version.h
sed -i 's/commit-[0-9a-f]*"/commit-'"$commit"'"/' version.h
git commit -m "Update version suffix to commit $commit" version.h

cleanup
