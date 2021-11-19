#!/bin/bash
 
filter_types=(mp4 avi gz zip tar) #定义过滤器，这些类型的文件会被过滤，黑名单
 
function filter(){
    file_name=$1
    typename=`basename ${file_name} | rev | cut -d "." -f 1 | rev` #rev将字符串逆序，此处即文件名逆序
    for i in ${filter_types[@]};do #@获取所有的值
        if [[ ${typename} == ${i} ]];then               #判断文件后缀
            echo "Filters On! ${file_name}"
            return 1
        fi
    done
}
 
function find_max_string_in_file(){ #从文件中查找最长的字符串
    if [[ ! -f ${1} ]];then  #判断是否为普通文件
        echo "$1 is not a regular file!"
        return
    fi
    filter $1 #调用函数filter
    if [[ $? -eq 1 ]];then  #$?上一条命令(filter $1)的返回值
        return
    fi
    echo "Finding Max_String in file $1"
    words=`cat $1 | tr -s -c "a-zA-Z" "\n"` #获取到一个文件中的所有字符串
    for i in ${words};do
        len_t=`echo -n ${i} | wc -c`   #获取到每个字符串的长度
        if [[ ${len_t} -gt ${len_max} ]];then
            len_max=${len_t}
            max_string=${i}
            max_file=$1
        fi
    done
}
 
function find_max_string_in_dir(){ #从目录中(如果是目录递归查找)的文件中查找最长的字符串
    for i in `ls -A $1`;do #ls -A $1获取到传入的目录中的目录和文件，-A去掉当前目录和上级目录
        if [[ -d ${1}/${i} ]];then  #如果是个目录
            echo "${1}/$i is a dir"
            find_max_string_in_dir ${1}/${i}
        else
            echo "${1}/${i} is a file"
            find_max_string_in_file ${1}/${i}
        fi
    done

len_max=0
max_string=""
max_file=""
 
if [[ $# -eq 0 ]];then
    find_max_string_in_dir "." #如果没写参数，就在当前目录进行查找操作
else
    for i in $@;do   #$@ 所有参数
        if [[ -d $i ]];then  #如果是目录
            find_max_string_in_dir $i
        else
            find_max_string_in_file $
        fi
    done
fi
#\033[31;34m %s \033[0m 给%s换颜色
echo "ok"









