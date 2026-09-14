. ./core.sh

LOGFILE=test_ok
NAME=aurelia
SALON=salut
SALON2=aurevoir

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +t\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON -t\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +k $PASSWORD\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON $PASSWORD\r\n"
#     printf "MODE &$SALON -k\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON +o $NAME\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON -o $NAME\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +l 1\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON -l\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK $NAME\r\n"
#     printf "USER $NAME $NAME $NAME :$NAME\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "MODE &$SALON -i\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +i\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "MODE" | wc -l)

end_test $count 5 $LOGFILE
