. ./core.sh

LOGFILE=test_err_433
TEMPFILE2=temp_file2

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1 &

sleep 1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE2"

nc $SERVER $PORT < "$TEMPFILE2" >> "$LOGFILE" 2>&1

rm $TEMPFILE

count=$(cat "$LOGFILE" | grep "433" | wc -l)

end_test $count 1 $LOGFILE
