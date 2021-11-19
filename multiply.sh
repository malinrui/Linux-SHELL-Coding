#!/bin/bash
# Program:
# Programmed for homework #2
# History:
# 2021/04/12 By Malr

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

num=${1}
[ "$#" -ne 1 ] && echo "Usage: multiply.sh Number" && exit 0

function multi(){
	base=1
	while [ $base -le ${1} ]
	do
		echo -ne "$((${base}*${2}))\t"
		base=$(($base+1))
	done
}

numtime=1
while [ $numtime -le ${1} ]
do
	if [ $numtime -eq $(($(($num+1))/2)) ]; then
		echo -ne "\v"
		numtime=$(($numtime+1))
		continue
	fi
	multi $num $numtime
	echo -e "\v"
	numtime=$(($numtime+1))
done
