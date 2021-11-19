#!/bin/bash
# Program:
# Programmed for final q4
# History:
# 2021/06/17 By Malr

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

read -p "input the size: " num

expr $num + 1 >/dev/null 2>&1
[ $? -ne 0 ] && echo "Not a positive number!" && exit 0
[ $num -le 0 ] && echo "Not a positive number!" && exit 0

counter=1
while [ $counter -le $num ]
do
	decounter=$(($num-$counter+1))
	while [ $decounter -ge 1 ]
	do
		echo -ne "${counter} "
		decounter=$(($decounter-1))
	done
	counter=$(($counter+1))
	echo ""
	bcounter=$(($counter-1))
	while [ $bcounter -gt 0 ]
	do
		echo -ne " "
		bcounter=$(($bcounter-1))
	done
done
echo ""
