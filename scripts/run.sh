#!/bin/bash
set -e

#APP_NAME="my_app"

#EXECUTABLE_PATH="./build/$APP_NAME"

#if [ ! -f "$EXECUTABLE_PATH" ]; then # 叹号放在[]内
#  echo "Executable not found, running build script first..."
#  ./scripts/build.sh
#fi

#"$EXECUTABLE_PATH"

#上面的代码是完全ok的，不过他只能在根目录中执行才行，接下来的版本我们来解决这个问题

PROJECT_ROOT="$(dirname "$(readlink -f "$0")")/.."
#readlink -f是获取文件绝对路径的方法
#dirname是去掉这个文件，变成这个文件所在的目录
#这个前面的执行完了变成/home/dayang/cpp_project_template/scripts/..
#这个..就是返回上一级，到当前工作区，有时候不需要..
BUILD_DIR="$PROJECT_ROOT/build"
APP_NAME="my_app"
EXECUTABLE_PATH="$BUILD_DIR/$APP_NAME"

if [ ! -f "EXECUTABLE_PATH" ]; then
  echo "Ececatable not found, running build script first ..."
  "$PROJECT_ROOT/scripts/build.sh"
  #用双引号把带变量的整个路径包起来更好，更安全
fi

"$EXECUTABLE_PATH"