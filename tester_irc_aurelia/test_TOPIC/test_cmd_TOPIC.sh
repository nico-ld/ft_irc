# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_cmd_TOPIC.sh                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afons <afons@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/12 15:34:35 by svogrig           #+#    #+#              #
#    Updated: 2026/09/14 15:35:46 by afons            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

. ./core.sh

LOGFILE=$(basename "$0" .sh)
SALON=salut
NAME=aurelia

start_test $LOGFILE

# doit envoyer TOPIC
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

# ne doit pas envoyer TOPIC
{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "TOPIC #$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# doit envoyer TOPIC
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

count=$(cat "$LOGFILE" | grep "TOPIC" | wc -l)

end_test $count 2 $LOGFILE