# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    utils.sh                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: svogrig <svogrig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/01 23:31:31 by svogrig           #+#    #+#              #
#    Updated: 2025/04/03 14:12:47 by svogrig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FG_BLACK="\033[30m"
FG_RED="\033[31m"
FG_GREEN="\033[32m"
FG_YELLOW="\033[33m"
FG_BLUE="\033[34m"
FG_PURPLE="\033[35m"
FG_CYAN="\033[36m"
FG_WHITE="\033[37m"
FG_DEFAULT="\033[39m"
FG_YELLOW="\033[33m"
FG_PURPLE="\033[35m"
FG_WHITE="\033[37m"
FG_DEFAULT="\033[39m"

title() {
	columns=60
	ln_before=$((($columns - 2 - ${#1})/2))
	ln_after=$(($columns - 2 - ${#1} - $ln_before))

	echo -n "$2"

	printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '*'

	printf "*%*s" $ln_before " "
	printf "%s" "$1"
	printf "%*s*\n" $ln_after ""

	printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '*'

	echo -n "$FG_DEFAULT"
}

subtitle() {
	columns=60
	ln_before=$((($columns - ${#1})/2))
	ln_after=$(($columns - ${#1} - $ln_before))

	echo -n "$FG_YELLOW"

	printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '-'

	printf "%*s" $ln_before " "
	printf "%s" "$1"
	printf "%*s\n" $ln_after ""

	printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '-'

	echo -n "$FG_DEFAULT"

	{
		columns=60
		ln_before=$((($columns - ${#1})/2))
		ln_after=$(($columns - ${#1} - $ln_before))

		echo -n "$FG_YELLOW"

		printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '-'

		printf "%*s" $ln_before " "
		printf "%s" "$1"
		printf "%*s\n" $ln_after ""

		printf "%*s\n" "${COLUMNS:-$columns}" '' | tr ' ' '-'
	} >> $ERROR_FILE
}

clean_file() {
	if [ -f $1 ]; then
		rm $1
	fi
}

count_in_file() {
	if [ -f $1 ]; then
		echo $(cat $1 | wc -c)
	else
		echo "0"
	fi
}
