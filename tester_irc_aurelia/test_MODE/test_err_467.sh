. ./core.sh

LOGFILE=test_err_467
SALON2=aurevoir
NAME=aurelia

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON2 $PASSWORD\r\n"
    printf "MODE &$SALON2 +k salutlameuf\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "467" | wc -l)

end_test $count 1 $LOGFILE
