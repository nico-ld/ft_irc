. ./core.sh

LOGFILE=test_err_412
NAME=aurelia

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "PRIVMSG $NAME :\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "412" | wc -l)

end_test $count 1 $LOGFILE
