#!/bin/bash
# Program:
# Programmed for homework #3
# History:
# 2021/04/19 By Malr

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

START=1
END=10000
sum=0
#定义数组
declare -a prime

function initPrime() {
    num=$1
    local i
    prime[1]=1
    #素数筛实现
    for (( i=2; i<=${num}; i++ ));do
        for (( j=$[ ${i} * ${i} ]; j<=${num}; j+=${i} ));do
            prime[$j]=1
        done
    done
}

initPrime ${END}

for (( i=${START}; i<=${END}; i++ ));do
    if [[ ${prime[$i]}x == x ]];then
        echo -ne "$i\t"
        let sum=$sum+$i
    fi  
done

echo -e "\n和为$sum" 
