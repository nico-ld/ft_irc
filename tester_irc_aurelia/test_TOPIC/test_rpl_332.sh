# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_rpl_332.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afons <afons@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/12 15:34:56 by svogrig           #+#    #+#              #
#    Updated: 2026/09/14 15:36:46 by afons            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

. ./core.sh

LOGFILE=$(basename "$0" .sh)
SALON=salut
NAME=aurelia

start_test $LOGFILE

# ne doit pas envoyer la reponse 332
{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +t\r\n"
    printf "TOPIC #$SALON :un salon tres propre\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1
echo "" >> "$LOGFILE"

# ne doit pas envoyer la reponse 332
{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "TOPIC #$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1
echo "" >> "$LOGFILE"

# doit envoyer la reponse 332
{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "MODE #$SALON +t\r\n"
    printf "TOPIC #$SALON :un salon pas propre\r\n"
    printf "TOPIC #$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

echo "${FG_RED}" >> "$LOGFILE"
cat "$TEMPFILE" >> "$LOGFILE"
echo "" >> "$LOGFILE"
echo "${FG_DEFAULT}" >> "$LOGFILE"

count=$(cat "$LOGFILE" | grep "332" | wc -l)

end_test $count 1 $LOGFILE