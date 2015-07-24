#!/bin/bash

build_results=()

function build_for {
  CC=$1
  CXX=$2
  BUILD_DIR=$CC

  mkdir $BUILD_DIR
  pushd $BUILD_DIR
  CC=$CC CXX=$CXX cmake ../..
  make
  build_results+=("$CC: $?")
  popd
}

if [[ -d builds ]]; then
  rm -rf builds
fi

mkdir builds
pushd builds

build_for clang-3.6 clang++-3.6
build_for gcc-5 g++-5
build_for clang clang++
build_for gcc-4.9 g++-4.9
build_for gcc-4.8 g++-4.8
build_for gcc-4.7 g++-4.7
build_for gcc-4.6 g++-4.6
build_for gcc-4.5 g++-4.5

popd

echo
echo "Result:"
for res in "${build_results[@]}"
do
    echo $res
done

echo "Done"
