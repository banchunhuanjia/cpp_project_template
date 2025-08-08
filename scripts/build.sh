#!/bin/bash
set -ex

BUILD_TYPE=${1:-"Debug"}
PROJECT_ROOT="$(dirname "$(readlink -f "$0")")/.."

echo "--- Building project in ${BUILD_TYPE} mode ---"

mkdir -p "$PROJECT_ROOT/build"
cd "$PROJECT_ROOT/build"

cmake -D CMAKE_BUILD_TYPE=$BUILD_TYPE ..
# 这个..是返回上一级目录的意思，目的是告诉cmake去哪里找CMakeLists.txt文件
make -j$(nproc)
# j是jobs，表示同时执行多个编译，nproc是当前机器可用的CPU核心

echo "--- Build finished suucessfully"