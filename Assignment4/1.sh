#!/bin/bash
ps aux | awk '{ print $2 }'  > p
sed -i 1d p
while read line           
do      
	if  ps -p $line >&- ;then
		sudo cat "/proc/$line/smaps" >> out
		ps -o minflt $line >> faults
		ps -o majflt $line >> faults
	fi
done < p  
rm p

