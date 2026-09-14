#!/bin/bash

. ./core.sh

LOGFILE=test_ok
TEMPFILE2=temp_file_jacqueline
NAME=aurelia
NAME2=jacqueline
SALON=salut

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME2\r\n"
    printf "USER $NAME2 $NAME2 $NAME2 :$NAME2\r\n"
} > "$TEMPFILE2"

nc $SERVER $PORT < "$TEMPFILE2" >> "$LOGFILE" 2>&1 &

sleep 2

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON\r\n"
    printf "INVITE $NAME2 &$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

rm "$TEMPFILE2"

count=$(cat "$LOGFILE" | grep "341" | wc -l)

end_test $count 1 $LOGFILE
