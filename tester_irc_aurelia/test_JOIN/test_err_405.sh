. ./core.sh

LOGFILE=test_err_405

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "USER aurelia 0 * :aurelia\r\n"
    printf "NICK aurelia\r\n"
    printf "JOIN #chanjoin_1\r\n"
    printf "JOIN #chanjoin_2\r\n"
    printf "JOIN #chanjoin_3\r\n"
    printf "JOIN #chanjoin_4\r\n"
    printf "JOIN #chanjoin_5\r\n"
    printf "JOIN #chanjoin_6\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "405" | wc -l)

if [ $count -ge 1 ]; then
	count=1
fi

end_test $count 1 $LOGFILE
