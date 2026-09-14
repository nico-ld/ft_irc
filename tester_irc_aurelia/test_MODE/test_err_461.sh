. ./core.sh

LOGFILE=test_err_461
NAME=aurelia
SALON=salut

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON\r\n"
    printf "MODE \r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON +k\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON +o\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON -o\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON +l\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "461" | wc -l)

end_test $count 1 $LOGFILE
