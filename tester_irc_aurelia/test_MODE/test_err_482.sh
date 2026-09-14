. ./core.sh

LOGFILE=test_err_482
SALON=salut
NAME=aurelia

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON $PASSWORD\r\n"
    printf "MODE #$SALON -o $NAME\r\n"
    printf "MODE #$SALON +i\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "482" | wc -l)

end_test $count 1 $LOGFILE
