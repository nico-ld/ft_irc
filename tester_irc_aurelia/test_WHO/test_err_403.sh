. ./core.sh

LOGFILE=test_err_403
SALON=salut
SALON2=aurevoir
SALON3=bye
NAME=aurelia

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK $NAME\r\n"
    printf "USER $NAME $NAME $NAME :$NAME\r\n"
    printf "JOIN &$SALON\r\n"
    printf "WHO &$SALON3\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"

nc $SERVER $PORT < "$TEMPFILE" > "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "403" | wc -l)

end_test $count 1 $LOGFILE
