#!/bin/bash
set -e

APP_NAME=${1:-"my_app"}

PID_FILE="/tmp/$APP_NAME.pid"

is_running(){
  if [ -f "$PID_FILE" ]; then
    PID=$(catch "$PID_FILE")
    if [ -n "$PID" ] && ps -p "$PID" > /dev/null 2>&1; then #这个ps是进程快照 -p参数表示查看特定进程，可以是一个或多个
      return 0
    fi
  fi
  return 1  
}


do_start(){
  echo "Starting $APP_NAME.."
  if is_running ; then
    echo "$APP_NAME is already running"
    exit 1
  fi
  echo "Simulating application start ..."
  sleep 10 &
  PID=$!
  echo $PID > "$PID_FILE"
  sleep 0.1
  echo "$APP_NAME started successfully with PID $(cat $PID_FILE)"
}

do_stop(){
  echo "Stopping $APP_NAME.."
  if ! is_running ;then
    echo "$APP_NAME is not running"
    return
  fi
  PID=$(cat "$PID_FILE")
  echo "Killing process with PID $PID ..."
  kill "$PID"
  rm "$PID_FILE"
  echo "$APP_NAME stopped"

}

do_status(){
  echo "Checking status of $APP_NAME.."
  if is_running; then
    PID=$(cat "$PID_FILE")
    echo "$APP_NAME is running with PID $PID"
  else 
    echo "$APP_NAME is not running"
  fi
}

usage(){
   echo -e "Usage: $0 {app_name} {start|stop|status}\n"
    echo "  app_name: (Optional) The name of the application. Defaults to 'my_app'."
    echo "  Commands:"
    echo "    start   - Start the application"
    echo "    stop    - Stop the application"
    echo "    status  - Check the application status"
    exit 1
}

ACTION=$2

case "$ACTION" in
  start)
    do_start
    ;;
  stop)
    do_stop #调用函数只写函数名
    ;;
  status)
    do_status
    ;;
  *)
    usage
    ;;
esac

exit 0