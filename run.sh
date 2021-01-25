#!/bin/bash
# **************************************************************************** #
#                                                                              #
#                                                         ::::::::             #
#    run.sh                                             :+:    :+:             #
#                                                      +:+                     #
#    By: thvan-de <thvan-de@student.codam.nl>         +#+                      #
#                                                    +#+                       #
#    Created: 2021/01/25 09:55:58 by thvan-de      #+#    #+#                  #
#    Updated: 2021/01/25 09:55:58 by thvan-de      ########   odam.nl          #
#                                                                              #
# **************************************************************************** #

RED='\033[0;31m'
GREEN='\033[0;32m'
ORANGE='\033[0;33m'
PURPLE='\033[0;35m'
LIGHTPURPLE='\033[1;35m'
CYAN='\033[0;36m'
LIGHTCYAN='\033[1;36m'
RESET='\033[0m'

function test {
	for var in "$@"
	do
		if [[ $var == "debug" ]]; then
			D="DEBUG=1"
		elif [[ $var == "g++" || $var == "clang++" ]]; then
			C="COMPILER=$var"
			echo -e "${LIGHTPURPLE}COMPILER = $var ${RESET}"
			echo -e "OSTYPE = $OSTYPE"
			if [[ $OSTYPE == *"darwin"* && $var == "g++" ]]; then
				C="COMPILER=$(brew --prefix)/bin/g++-10"
			fi
		elif [[ $var == "time" ]]; then
			TIME=$var
		elif [[ $var == "leaks" ]]; then
			LEAKS=$var
		fi
	done
	make fuckingclean


echo -e "${GREEN}Compiling and testing for our class $1 ${RESET}"
	make "$1" $C $D && ./webserv.out "$TIME" "$LEAKS" > tests/webserv.txt #2>&1
	STATUS_WEBSERV=$?

#	echo -e "${ORANGE}Startin testing for $1${RESET}"

	diff tests/webserv.txt answers/"$1_answers.txt" > tests/diff.txt;
	if [ $? -eq 1 ]; then
		echo -e "${RED}Diff failed${RESET}"
		cat tests/diff.txt
		exit 1
	else
		echo -e "${LIGHTPURPLE}Diff found no differences${RESET}"
	fi	
}

declare -a arr=("response")
ARG="all"
for var in "$@"
do
	for container in "${arr[@]}"
	do
		if [[ $var == "$container" ]]; then
			ARG=$container
		fi
	done
done

if [[ $ARG == "all" ]]; then
	for i in "${arr[@]}"
	do
		test "$i" "$1" "$2" "$3"
	done
else
	test "$ARG" "$2" "$3" "$4"
fi	