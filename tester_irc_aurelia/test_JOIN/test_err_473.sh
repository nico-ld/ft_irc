#!/bin/bash

. ./core.sh

LOGFILE=test_err_473
TEMPFILE2=temp_file_err_473_jacqueline

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN &salut\r\n"
    printf "MODE &salut +i\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1 &

sleep 2

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK jacqueline\r\n"
    printf "USER jacqueline jacqueline jacqueline :jacqueline\r\n"
    printf "JOIN &salut\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE2"
nc $SERVER $PORT < "$TEMPFILE2" >> "$LOGFILE" 2>&1

rm "$TEMPFILE2"

count=$(cat "$LOGFILE" | grep "473" | wc -l)

end_test $count 1 $LOGFILE
