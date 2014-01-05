#!/bin/bash
if [ -n "$1" ]
then
	gpg -d $1
	
else
	echo 'No file name entered'
fi
