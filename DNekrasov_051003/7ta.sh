#!/bin/bash

# $1 = C-file path.
# $2 = exe-file path.

RED='\033[0;31m'
NC='\033[0m'

if [ $# -eq 2 ]
then
	if [ ! -f $1 ] 
	then
		{
			echo -e "${RED}!ERROR!${NC}"
			echo "$1 : this file does not exist"
		}>&2
	else
		gcc $1 -o "$2.exe" && ./$2.exe
	fi
else
	{
		echo -e "${RED}!ERROR!${NC}"
		echo "Firstargument = C-file to compile"
		echo "Second argument = exe-file output"
	}>&2
fi
