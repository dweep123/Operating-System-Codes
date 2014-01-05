#!/bin/bash
sed -i "1 i function rm(){" ~/.bashrc
sed -i "2 i 	ls -i \$\* | awk \'{print \$\1}\' > ww" .bashrc
sed -i "3 i 	for line in \$(cat ww)" .bashrc
sed -i "4 i 		do" .bashrc
sed -i "5 i 	find . -inum \$line -exec mv {} /home/\$USER/.local/share/Trash/files/ \\\; 2> .ww" .bashrc
sed -i "6 i 	done" .bashrc
sed -i "7 i 	mv ww ~/Trash" .bashrc
sed -i "8 i 	mv .ww ~/Trash" .bashrc
sed -i "9 i }" .bashrc
