#!/bin/bash

set -e

BUILD_DIR="build"

echo "Cecking if directory '$BUILD_DIR' exists ..."
# echo搭配""时双引号的意思是对变量进行填充，echo单独搭配''就是不会填充替换，而这个语句的意思其实是想让这个build外有个双引号，因为主题是外面的双引号，一定会触发填充

if [ -d "$BUILD_DIR" ]; then # []的两边必须有空格，if和[]之间也要有空格
  echo "--> Success"
  echo "--> Contents:"
  ls "$BUILD_DIR"
else
  echo "--> not found"
fi

echo "Finished"