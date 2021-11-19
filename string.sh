#!/bin/bash
# Program:
# Programmed for homework #4
# History:
# 2021/04/26 By Malr

PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH

maxlen=0
maxname=0
maxstr=0
maxpath=0
colum=0

function counts() {  #实现获取字符串长度，比大小
    for i in `cat $1 | tr -s -c "a-zA-Z""\t"`;do  
        if [ $maxlen -lt ${#i} ];then
            maxlen=${#i}
            maxname=$1
            maxstr=$i
            maxpath=$2
        fi
    done    
    grep -n $maxstr $1
}

function cycle() {
    path=$1
    cd $path
    for filename in `ls`;
    do
        if [ -d "$filename" ];then  #如果该文件名是目录
            cycle "$path/$filename/" #一直循环执行cd目录这个过程 
        elif [ -f "$filename" ];then  #如果该文件名不是目录
            counts $filename $path #遍历此文件中的字符串
        fi    
    done   
    cd ../  #如果没有找到文件，则回到上一级
}

if [ "$#" -ne 1 ]; then
	echo "Using default path"
	path=$(pwd)
else
	path=${1}
fi

cycle $path
echo "max length:" $maxlen
echo "the longest string:" $maxstr
echo "path:" $maxpath $maxname
echo "position:" $colum
