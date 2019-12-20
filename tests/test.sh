#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
WHITE='\033[1;37m'

Make -C ../
cp ../ft_ssl .

if [ -z "$1" ]
then
	echo "Usage: sh test.sh [hash]"
else
	while read p; do
		echo "$WHITE"
		echo "$p"
		echo "$p" | openssl $1 > real.txt
		echo "$p" | ./ft_ssl $1 > mine.txt
		cat real.txt

		DIFF=$(diff real.txt mine.txt)
		if [ "$DIFF" != "" ] 
		then
			echo "$RED"
			echo $DIFF
			echo "KO"
		else
			echo "$GREEN"
			echo "OK"
		fi
	done < some_messages.txt

	FILES=test_files/*
	for f in $FILES ; do
		echo "$WHITE"
		echo "$f"
		cat "$f" | openssl $1 > real.txt
		./ft_ssl $1 -q "$f" > mine.txt

		DIFF=$(diff real.txt mine.txt)
		if [ "$DIFF" != "" ] 
		then
			echo "$RED"
			echo $DIFF
			echo "KO"
		else
			echo "$GREEN"
			echo "OK"
		fi
	done

	echo "$WHITE"
	cmds=(
		"echo \"pickle rick\" | ./ft_ssl $1"
		"echo \"Do not pity the dead, Harry.\" | ./ft_ssl $1 -p"
		"echo \"Pity the living.\" | ./ft_ssl $1 -q -r"
		"echo \"And above all,\" > file"
		"echo \"be sure to handle edge cases carefully\" | ./ft_ssl $1 -p file"
		"echo \"some of this will not make sense at first\" | ./ft_ssl $1 file"
		"echo \"but eventually you will understand\" | ./ft_ssl $1 -p -r file"
		"echo \"GL HF let's go\" | ./ft_ssl $1 -p -s \"foo\" file"
		"echo \"one more thing\" | ./ft_ssl $1 -r -p -s \"foo\" file -s \"bar\""
		"echo \"just to be extra clear\" | ./ft_ssl $1 -r -q -p -s \"foo\" file"
	)

	for (( i = 0; i < ${#cmds[@]} ; i++ )); do
		printf "\n${cmds[$i]}\n"
		eval "${cmds[$i]}"
	done

	rm -f file
fi
