. ./core.sh

LOGFILE=test_err_451
NAME=aurelia

start_test $LOGFILE

{
    printf "PRIVMSG &$NAME :salut la mif !\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "PRIVMSG &$NAME :salut la mif !\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "PRIVMSG &$NAME :salut la mif !\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "PRIVMSG &$NAME :salut la mif !\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "451" | wc -l)

end_test $count 4 $LOGFILE
