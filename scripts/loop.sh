#!/bin/bash
set -e

for fruit in apple banana orange; do
  echo "Current fruit is $fruit"
done

echo ""

for file in *;do
  if [ -f "$file" ]; then
    echo "Found a file $file"
  fi
done

echo ""

for ((i=1;i<=5;i++)); do
  echo "counting $i"
done

echo ""

for script_name in $(ls -1 scripts); do
  echo "Found a script: scripts/$script_name"
done
#for var in $(command);
#ls -l 长列表格式 -1 单列格式