# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    core.sh                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nico <nico@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/02 01:58:42 by svogrig           #+#    #+#              #
#    Updated: 2026/09/14 18:32:40 by nico             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

. ./utils.sh
. ./settings.sh

TEMPFILE=temp_file

start_test() {
	printf "${FG_YELLOW}"
	printf "%16s" "$1"
	printf " : "
	if [ -f "$1" ]; then
		rm "$1"
	fi
	{
		printf "${FG_YELLOW}"
		printf "%16s" "$1"
		printf " : "
	} >> $ERROR_FILE
	printf "1" >> $NBR_TEST
}

end_test() {

	rm $TEMPFILE
	if [ "$1" -eq $2 ]; then
		printf "${FG_GREEN}OK${FG_DEFAULT}\n"
		{
			printf "${FG_GREEN}OK${FG_DEFAULT}\n"
		} >> $ERROR_FILE
		printf "1" >> $NBR_SUCCESS
	else
		printf "${FG_RED}KO${FG_DEFAULT}\n"
		{
			printf "${FG_RED}KO${FG_DEFAULT}\n"
			cat $3
		} >> $ERROR_FILE
		printf "1" >> $NBR_FAIL
	fi
	rm $3
}
