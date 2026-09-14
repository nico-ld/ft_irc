#!/bin/bash

. ./core.sh

LOGFILE=test_err_482
NAME=aurelia
NAME2=jacqueline
SALON=salut

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME2\r\n"
    printf "USER $NAME2 $NAME2 $NAME2 :$NAME2\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1 &

sleep 2

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON\r\n"
    printf "MODE &$SALON -o $NAME\r\n"
    printf "INVITE $NAME2 &$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "482" | wc -l)

end_test $count 1 $LOGFILE
