SERVER=127.0.0.1
PORT=8080
LOGFILE=test_output_wt_space

start_test $LOGFILE
#{
#    printf ""
#} > "$TEMPFILE"
#
#nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

{
    printf "salut\r\nlamif\r\n"
    printf "\n"
    printf "salut\r\nlamif\r\n"
    printf "\r"
    printf "salut\r\nlamif\r\n"
    printf "\r\n"
    printf "salut\r\nlamif\r\n"
    printf "\n\r"
    printf "salut\r\nlamif\r\n"
    printf "QUIT"
    printf "QUIT\r"
    printf "QUIT\n"
    printf "QUIT\r\n"


} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "421" | wc -l)

if [ "$count" -eq 8 ]; then
    echo "${GREEN}$LOGFILE : OK${RESET}"
else
    echo "${RED}$LOGFILE : KO${RESET}"
    cat $LOGFILE
fi

rm $LOGFILE
