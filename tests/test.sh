#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
WHITE='\033[1;37m'

Make -C ../
cp ../ft_ssl .


size=0
if [ -z "$1" ]
then
	echo "Usage: sh test.sh [hash]"
else
	# TODO: size for md5
	if [ $1 = "sha256" ]
	then
		size=64
	fi
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
		openssl $1 -r "$f" | cut -c1-$size > real.txt
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
fi
