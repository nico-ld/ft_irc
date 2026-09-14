. ./core.sh

LOGFILE=test_ok
NAME=aurelia
SALON=gourde

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON\r\n"
    printf "WHO &$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "352" | wc -l)

end_test $count 1 $LOGFILE

