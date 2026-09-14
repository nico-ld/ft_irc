. ./core.sh

LOGFILE=test_err_464

start_test $LOGFILE

{
    printf "PASS salutlameuf\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "464" | wc -l)

end_test $count 1 $LOGFILE
