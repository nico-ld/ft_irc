. ./core.sh

LOGFILE=test_err_482
NAME=aurelia
SALON=salut

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +t\r\n"
    printf "MODE #$SALON -o $NAME\r\n"
    printf "TOPIC #$SALON :un salon tres propre\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "482" | wc -l)

end_test $count 1 $LOGFILE
