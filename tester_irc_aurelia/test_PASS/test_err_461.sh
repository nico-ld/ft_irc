. ./core.sh

LOGFILE=test_err_461

start_test $LOGFILE

{
    printf "PASS \r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "461" | wc -l)

end_test $count 1 $LOGFILE
