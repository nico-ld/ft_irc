#!/bin/bash

. ./core.sh

LOGFILE=test_ok
TEMPFILE=temp_file_ok_aurelia
TEMPFILE2=temp_file_ok_jacqueline

start_test $LOGFILE
#{
#    printf "PASS $PASSWORD\r\n"
#    printf "NICK aurelia\r\n"
#    printf "USER aurelia aurelia aurelia :aurelia\r\n"
#    printf "JOIN &salut salut\r\n"
#    printf "QUIT\r\n"
#} > "$TEMPFILE"

#nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1 &

#sleep 2

#{
#    printf "PASS $PASSWORD\r\n"
#    printf "NICK jacqueline\r\n"
#    printf "USER jacqueline jacqueline jacqueline :jacqueline\r\n"
#    printf "JOIN &salut salut\r\n"
#    printf "QUIT\r\n"
#} > "$TEMPFILE2"

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN &salut salut\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE2"

nc $SERVER $PORT < "$TEMPFILE2" >> "$LOGFILE" 2>&1

rm "$TEMPFILE2"

#rm "$TEMPFILE" "$TEMPFILE2"

count=$(cat "$LOGFILE" | grep "JOIN" | wc -l)

end_test $count 2 $LOGFILE

