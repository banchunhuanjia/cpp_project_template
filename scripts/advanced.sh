#!/bin/bash
set -e

log_info(){
  echo "[INFO] $1"
}

command_exists(){
  command -v "$1" >/dev/null 2>&1
}
# command -v [cmd]用于检查命令是否存在于PATH

main(){
  log_info "Advanced script started"

  log_info "Checking for required commands..."
  if command_exists "cmake"; then
    log_info "--> 'cmake' command found"
  else 
    log_info "--> Warning: 'cmake' command not found"
  fi

  if command_exists "non_existent_command"; then
    log_info "--> 'non_existent_command' found"
  else
    log_info "--> 'non_existent_command' not found, as expected"
  fi

  echo ""

  counter=5
  while [ $counter -gt 0 ]; do
    echo "Countdown: $counter"
    ((counter--)) #算数运算的标准方式就是(())
    sleep 1
  done

  log_info "Countdown finished"

  # while read line; do echo "$line"; done< file.txt 逐行读取文件
  # 一次性写入
  {
    echo "#!/bin/bash"
    for i in {1..5}; do
      echo "echo 'This is line $i'"
      #这里不能是echo "echo "This is line $i"",因为shell解析有就近配对，这个写进去的结果是echo
      #还有一点就是i已经在这个脚本内填充好了，写到generated_script.sh内就没有i了，不用担心没填充的问题
      #不过你可以用echo "echo \"This is line $i\"",给双引号加个转义符
    done
  } > generated_script.sh
}

main