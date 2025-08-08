#!/bin/bash

set -e

APP_NAME=${1:-"my_app"}

PID_FILE="/tmp/$APP_NAME.pid"

ACTION=$2

case "$ACTION" in
  start)
    echo "Starting $APP_NAME.."
    if [ -f "$PID_FILE" ]; then
      echo "Error: $APP_NAME is already running (PID file exits: $PID_FILE)"
      exit 1
    fi

    echo "Simulating application start..."
    sleep 10 &
    PID=$!
    echo $PID > "$PID_FILE"

    sleep 0.1 #给后台进程启动时间的经验措施
    echo "$APP_NAME started successfully with PID $(cat $PID_FILE)"
    ;;

  stop)
    echo "Stopping $APP_NAME.."
    if [ ! -f "$PID_FILE" ]; then
      echo "Warning: $APP_NAME is not running (PID file not found)"
    else
      PID=$(cat "$PID_FILE")
      echo "Killing process with PID $PID..."
      kill "$PID"
      rm "$PID_FILE"
      echo "$APP_NAME stopped"
    fi
    ;;

  status)
    echo "Checking status of $APP_NAME.."
    if [ -f "$PID_FILE" ]; then
      PID=$(cat "$PID_FILE")
      if ps -p "$PID" > /dev/null 2>&1; then
      #这是一个I/O重定向
      #linux在程序运行时打开0，1，2三个管道
      #>的作用是输出重定向
      #是处理脚本噪声的好方法
        echo "--> $APP_NAME is running with PID $PID"
      else
        echo "--> Warning: PID file exits, but process with PID $PID is not running. Cleaning up stale PID file"
        rm "$PID_FILE"
      fi    
    else
      echo "--> $APP_NAME is not running"
    fi
    ;;
  *)
    echo "Usage: $0 {app_name} {start|stop|status}"
    exit 1
    ;;
esac

exit 0