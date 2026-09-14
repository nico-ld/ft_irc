. ./core.sh

LOGFILE=test_err_461
SALON=salut
NAME=aurelia

start_test $LOGFILE

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK aurelia\r\n"
#     printf "USER aurelia aurelia aurelia :aurelia\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "TOPIC &$SALON un salon tres propre\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN &$SALON\r\n"
    printf "TOPIC\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK aurelia\r\n"
#     printf "USER aurelia aurelia aurelia :aurelia\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "TOPIC $SALON\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS $PASSWORD\r\n"
#     printf "NICK aurelia\r\n"
#     printf "USER aurelia aurelia aurelia :aurelia\r\n"
#     printf "JOIN &$SALON\r\n"
#     printf "TOPIC :$SALON\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"
# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "461" | wc -l)

end_test $count 1 $LOGFILE
