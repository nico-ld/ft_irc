. ./core.sh

LOGFILE=test_err_472
SALON=salut
NAME=aurelia

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON $PASSWORD\r\n"
    printf "MODE #$SALON +ug\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON $PASSWORD\r\n"
#     printf "MODE &$SALON +l huit\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "472" | wc -l)

end_test $count 2 $LOGFILE
