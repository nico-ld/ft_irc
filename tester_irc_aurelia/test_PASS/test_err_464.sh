. ./core.sh

LOGFILE=test_err_464

start_test $LOGFILE

{
    printf "PASS salut\r\n"
    printf "PASS salutlamifcavaoubienmoicava\r\n"
    printf "PASS salut@\r\n"
    printf "PASS salut:\r\n"
    printf "PASS salut!\r\n"
    printf "PASS salut\t\r\n"
    printf "PASS sa\rlut\r\n"
    printf "PASS sa\nlut\r\n"
    printf "PASS :salut la mif\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# #trop court
# {
#     printf "PASS salut\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"

# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# #trop long
# {
#     printf "PASS salutlamifcavaoubienmoicava\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"

# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# #caractere_interdit " @:!\t\r\n\0"
# {
#     printf "PASS salut@\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"

# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# {
#     printf "PASS :salut la mif\r\n"
#     printf "QUIT\r\n"
# } > "$TEMPFILE"

# nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

count=$(cat "$LOGFILE" | grep "464" | wc -l)

end_test $count 9 $LOGFILE
