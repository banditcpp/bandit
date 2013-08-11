#!/bin/bash

function build_for {
  CC=$1
  CXX=$2
  BUILD_DIR=build-$CC

  mkdir $BUILD_DIR
  pushd $BUILD_DIR
  CC=$CC CXX=$CXX cmake ../..
  make
  popd
}

if [[ -d builds ]]; then
  rm -rf builds
fi

mkdir builds
pushd builds

build_for gcc-4.5 g++-4.5
GCC45=$?

build_for gcc-4.6 g++-4.6
GCC46=$?

build_for gcc-4.7 g++-4.7
GCC47=$?

build_for gcc-4.8 g++-4.8
GCC48=$?

build_for clang clang++
CLANG=$?

popd

echo
echo "Result:"
echo -e "gcc-4.5:\t$GCC45"
echo -e "gcc-4.6:\t$GCC46"
echo -e "gcc-4.7:\t$GCC47"
echo -e "gcc-4.8:\t$GCC48"
echo -e "clang:\t$CLANG"
echo "Done"
exit $(( $GCC45 + $GCC46 + $GCC47 + $GCC48 + $CLANG ))
