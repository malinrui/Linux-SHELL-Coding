#!/bin/bash
# Program:
# Programmed for final q4
# History:
# 2021/06/17 By Malr

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

read -p "input the min size: " num_min
read -p "input the max size: " num_max

expr $num_min + 1 >/dev/null 2>&1
[ $? -ne 0 ] && echo "Not a positive number!" && exit 0
[ $num_min -le 0 ] && echo "Not a positive number!" && exit 0

expr $num_max + 1 >/dev/null 2>&1
[ $? -ne 0 ] && echo "Not a positive number!" && exit 0
[ $num_max -le 0 ] && echo "Not a positive number!" && exit 0

counter=1
while [ $(($counter+$num_min-1)) -le $num_max ]
do
	decounter=$(($num_max-$counter+1))
	while [ $decounter -ge 1 ]
	do
		echo -ne "$(($counter+$num_min-1)) "
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
