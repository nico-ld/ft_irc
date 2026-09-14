. ./core.sh

LOGFILE=test_err_403

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN &salut\r\n"
    printf "KICK &aurevoir aurelia\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "403" | wc -l)

end_test $count 1 $LOGFILE
