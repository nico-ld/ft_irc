. ./core.sh

# init ***************************************

ERROR_FILE="error"
NBR_TEST="nbr_test"
NBR_FAIL="nbr_fail"
NBR_SUCCESS="nbr_success"

clean_file $ERROR_FILE
clean_file $NBR_TEST
clean_file $NBR_FAIL
clean_file $NBR_SUCCESS

export ERROR_FILE
export NBR_TEST
export NBR_FAIL
export NBR_SUCCESS

# titre **************************************

title "IRC tester" $FG_PURPLE

# start serveur ******************************

gnome-terminal -- ./../ircserv $PORT $PASSWORD

# exec ************************************

if [ $# = 0 ]; then
	sh ./test_PASS/test_all.sh
	sh ./test_USER/test_all.sh
	sh ./test_NICK/test_all.sh
	sh ./test_JOIN/test_all.sh
	sh ./test_KICK/test_all.sh
	sh ./test_INVITE/test_all.sh
	sh ./test_PRIVMSG/test_all.sh
	sh ./test_TOPIC/test_all.sh
	sh ./test_MODE/test_all.sh
	#sh ./test_WHO/test_all.sh
	sh ./test_QUIT/test_all.sh
else
	for i in $@
	do
		cmd=$(echo $i | tr '[:lower:]' '[:upper:]')
		case "$cmd" in
			"PASS") sh ./test_PASS/test_all.sh ;;
			"USER") sh ./test_USER/test_all.sh ;;
			"NICK") sh ./test_NICK/test_all.sh ;;
			"JOIN") sh ./test_JOIN/test_all.sh ;;
			"KICK") sh ./test_KICK/test_all.sh ;;
			"INVITE") sh ./test_INVITE/test_all.sh ;;
			"PRIVMSG") sh ./test_PRIVMSG/test_all.sh ;;
			"TOPIC") sh ./test_TOPIC/test_all.sh ;;
			"MODE") sh ./test_MODE/test_all.sh ;;
			"WHO") sh ./test_WHO/test_all.sh ;;
			"QUIT") sh ./test_QUIT/test_all.sh ;;
			*) subtitle "$cmd : unknow command"
		esac
	done
fi

# stats ***************************************

nbr=$(count_in_file $NBR_TEST)
fail=$(count_in_file $NBR_FAIL)
success=$(count_in_file $NBR_SUCCESS)

echo "\nsuccess: ${FG_GREEN}$success${FG_DEFAULT} / $nbr - fail: ${FG_RED}$fail${FG_DEFAULT} / $nbr"

# clean ***************************************

clean_file $NBR_TEST
clean_file $NBR_FAIL
clean_file $NBR_SUCCESS
