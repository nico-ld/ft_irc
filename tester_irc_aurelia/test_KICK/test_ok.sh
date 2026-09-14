. ./core.sh

LOGFILE=test_ok

start_test $LOGFILE

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK jacqu\r\n"
    printf "USER jacqu jacqu jacqu :jacqu\r\n"
    printf "JOIN #salut\r\n"
    printf "KICK #salut jacqu\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "PASS $PASSWORD\r\n"
    printf "NICK jacqu\r\n"
    printf "USER jacqu jacqu jacqu :jacqu\r\n"
    printf "JOIN #salut\r\n"
    printf "KICK #salut jacqu :degage toi meme\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "KICK" | wc -l)

end_test $count 2 $LOGFILE
