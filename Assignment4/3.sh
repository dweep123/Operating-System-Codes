#!/bin/bash
if [ -n "$1" ]
then
	gpg -c $1
	
else
	echo 'No file name entered'
fi
