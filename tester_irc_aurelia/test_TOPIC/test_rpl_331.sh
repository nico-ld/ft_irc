# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test_rpl_331.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: afons <afons@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/12 16:00:38 by svogrig           #+#    #+#              #
#    Updated: 2026/09/14 15:36:25 by afons            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

. ./core.sh

LOGFILE=$(basename "$0" .sh)
SALON=salut
NAME=aurelia

start_test $LOGFILE

# ne doit pas envoyer la reponse 331_NOTOPIC
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

# doit envoyer la reponse 331_NOTOPIC
{
    printf "PASS $PASSWORD\r\n"
    printf "NICK aurelia\r\n"
    printf "USER aurelia aurelia aurelia :aurelia\r\n"
    printf "JOIN #$SALON\r\n"
    printf "TOPIC #$SALON\r\n"
    printf "QUIT\r\n"
} > "$TEMPFILE"
nc $SERVER $PORT < "$TEMPFILE" >> "$LOGFILE" 2>&1

# ne doit pas envoyer la reponse 331_NOTOPIC
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

count=$(cat "$LOGFILE" | grep "331" | wc -l)

end_test $count 1 $LOGFILE