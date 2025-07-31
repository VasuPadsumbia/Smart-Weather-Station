#!/bin/sh
### BEGIN INIT INFO
# Provides:          weather_cpp_server
# Required-Start:    $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Start weather C++ server daemon at boot time
### END INIT INFO

DAEMON_PATH="/opt/smart_weather_station"
DAEMON="$DAEMON_PATH/weather_cpp_server"
PIDFILE="/var/run/weather_cpp_server.pid"
LOGFILE="/var/log/weather_cpp_server.log"

case "$1" in
  start)
    echo "Starting weather_cpp_server..."
    start-stop-daemon --start --background --make-pidfile --pidfile $PIDFILE --exec $DAEMON
    ;;
  stop)
    echo "Stopping weather_cpp_server..."
    start-stop-daemon --stop --pidfile $PIDFILE
    ;;
  restart)
    $0 stop
    sleep 1
    $0 start
    ;;
  *)
    echo "Usage: $0 {start|stop|restart}"
    exit 1
esac

exit 0
