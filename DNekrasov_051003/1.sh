#!/bin/bash

if [ $# -ne 3 ]
then
	echo Not enough arguments
	
else

find $name -type f -size +$1c -size -$2c -printf '%p %s\n' >$3

find $name -type f -size +$1c -size -$2c -printf '%p %s\n' | wc -l

fi
