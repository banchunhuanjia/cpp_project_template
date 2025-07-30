# 拉取新代码 -> 编译 -> 重启服务

# 拉取
# 编译
# 终止原本的运行
# 部署新文件因为build.sh的执行结果并不在install内，我们要复制一下
# 重启
#!/bin/bash
set -e

PROJECT_ROOT="$(dirname "$(readlink -f "$0")")/.."
APP_NAME="my_app"
INSTALL_DIR="$PROJECT_ROOT/install"

main(){
  cd "$PROJECT_ROOT"
  git pull origin master

  "$PROJECT_ROOT/scripts/build.sh"

  PID_FILE="/tmp/$APP_NAME.pid"

  if [ -f "$PID_FILE" ]; then
    kill "$(cat "$PID_FILE")"
    rm "$PID_FILE"
    sleep 1
  fi

  cp -f "$PROJECT_ROOT/build/$APP_NAME" "$INSTALL_DIR"
  #-f参数是如果存在那么这个复制会强制覆盖
  
  cd "$INSTALL_DIR"
  ./"$APP_NAME" &
  PID=$!
  echo "$PID" > "$PID_FILE"

}

main