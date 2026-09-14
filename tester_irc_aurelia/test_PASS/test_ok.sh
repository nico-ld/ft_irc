. ./core.sh

LOGFILE=test_ok

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "NOTICE" | wc -l)

end_test $count 1 $LOGFILE
