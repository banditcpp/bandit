#!/bin/sh

init() {
  build_dir=$1
  cc=$2
  cxx=$3
  type=$4
  if [[ -d ${build_dir} ]]; then
    echo "Deleting directory ${build_dir} ..."
    rm -rf ${build_dir}
  fi

  mkdir ${build_dir}
  pushd ${build_dir}
  CC=$cc CXX=$cxx cmake -DCMAKE_BUILD_TYPE=$type ../bandit
  popd
}

init ../build-clang-debug clang clang++ debug
init ../build-clang clang clang++ release
init ../build-gcc gcc g++ release
