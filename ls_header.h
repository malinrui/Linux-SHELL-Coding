#include <stdio.h>
#include <dirent.h>	// for DIR, dirent
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "ls_util.h"
 
// struct
struct attribute file_attribute[200]; // maximum 200
 
void lsLong(char *dirname)
{
	
	DIR *mydir = opendir( dirname );			/* directory */	
 
	char filename[20];
	int file_num = 0;
 
	if (mydir == NULL) {
		// 显示单个文件的详细信息
		strcpy(filename, dirname);
		ls_long_file("./", filename, &file_attribute[0]);
		++ file_num;
	} else {
		// 考虑用户输入文件夹没有输入反斜杠的情况
		int len = strlen(dirname);
		if (dirname[len - 1] != '/')
		{
			dirname[len] = '/';
			dirname[len+1] = '\0';
		}
 
		// 循环得到当前目录下的所有文件名,并存储在自定义的结构体中
		struct dirent *mydirent;			/* file */
		while ( (mydirent = readdir( mydir )) != NULL) {
			char filename[20];
			strcpy(filename, mydirent->d_name);
			// 不能为隐藏文件
			if (!strcmp(filename, ".") || !strcmp(filename, ".") || filename[0] != '.') {
				ls_long_file(dirname, filename, &file_attribute[file_num ++]);
			}	
			
		}
		closedir( mydir );
	}
 
	// 按照文件名排序
	struct attribute temp;
	char filename1[20];
	char filename2[20];
	int i, j;
 
	for (i = 0; i < file_num; ++i)
	{
		for (j = i + 1; j < file_num; ++ j)
		{
			strcpy(filename1, file_attribute[i].filename);
			strcpy(filename2, file_attribute[j].filename);
			if ( strcmp(filename1, filename2) > 0)
			{
				temp = file_attribute[i];
				file_attribute[i] = file_attribute[j];
				file_attribute[j] = temp;
			}
		}			
	}
 
	// 格式化输出时,考虑每个属性值的范围
	int max_mode = 0;
	int max_links = 0;
	int max_user_name = 0;
	int max_group_name = 0;
	int max_size = 0;
	int max_mtime = 0;
	int max_filename = 0;
	int max_extra = 0;
 
	for (i = 0; i < file_num; ++ i)
	{
		if ( max_mode < strlen(file_attribute[i].mode) ) {
			max_mode = strlen(file_attribute[i].mode);
		}
 
		if (max_links < f(file_attribute[i].links)) {
			max_links = f(file_attribute[i].links);
		}
 
		if ( max_user_name < strlen(file_attribute[i].user_name) ) {
			max_user_name = strlen(file_attribute[i].user_name);
		}
 
		if ( max_group_name < strlen(file_attribute[i].group_name) ) {
			max_group_name = strlen(file_attribute[i].group_name);
		}
 
		if (max_size < f(file_attribute[i].size)) {
			max_size = f(file_attribute[i].size);
		}
 
		if ( max_mtime < strlen(file_attribute[i].mtime) ) {
			max_mtime = strlen(file_attribute[i].mtime);
		}
 
		if ( max_filename < strlen(file_attribute[i].filename) ) {
			max_filename = strlen(file_attribute[i].filename);
		}
 
		if ( max_extra < strlen(file_attribute[i].extra) ) {
			max_extra = strlen(file_attribute[i].extra);
		}	
	}
 
	for (i = 0; i < file_num; ++i)
	{
		char format[50];
		// 定义输出的格式
		sprintf(format, "%%%ds %%%dd %%%ds %%%ds %%%dld %%%ds %%s%%s\n", 
			max_mode, max_links, max_user_name, max_group_name, max_size,
			max_mtime);
		// 按照定义的输出格式输出
		printf(format, file_attribute[i].mode, file_attribute[i].links, 
			file_attribute[i].user_name, file_attribute[i].group_name, file_attribute[i].size, 
			file_attribute[i].mtime, file_attribute[i].filename, file_attribute[i].extra);
	}
 
}
 
// 处理不带 -l 参数的 ls 命令
void lsShort(char *dirname) {
 
	DIR *mydir = opendir( dirname );			/* directory */	
 
	// 用来暂时存储要显示的目录下的所有文件名,可以看到最大可以支持200个文件,但是每个文件名最长为20
	char filenames[200][20];
	int file_num = 0;
 
	if (mydir == NULL) {
		// 直接显示该文件
		printf("%s\n\n", dirname);
		return ;
	} else {
		// 循环检查下面有多少文件,并把文件名全部放到filenames数组里
		struct dirent *mydirent;			/* file */
		while ( (mydirent = readdir( mydir )) != NULL) {
			char fname[20];
			strcpy(fname, mydirent->d_name);	
			if (fname[0] != '.' ) {
				strcpy(filenames[file_num], mydirent->d_name);
				file_num ++;
			}	
		}
		closedir( mydir );
	}
 
	// 文件名排序
	int i, j;
	char temp[20];
	for(i = 0; i < file_num; ++ i) {
		for(j = i+1; j < file_num; ++ j) {
			if(strcmp(filenames[i], filenames[j]) > 0) {
				strcpy(temp, filenames[i]);
				strcpy(filenames[i], filenames[j]);
				strcpy(filenames[j], temp);
			}
		}
	}
 
	// 确定所有文件里面最长的文件名的长度
	int max_len = 0;
	
	for(i = 0; i < file_num; ++ i) {
		int len = strlen(filenames[i]);
		if(len > max_len) {
			max_len = len;
		}
	}
 
	// 得到当前终端的分辨率
	int cols = 80;
	int lines = 24;
	getTerminatorSize(&cols, &lines);
	
	char format[20];
	sprintf(format, "%%-%ds  ", max_len);
 
	// 格式化输出,当长度大于终端的列数时,换行
	int current_len = 0;
	for(i = 0; i < file_num; ++ i) {
		printf(format, filenames[i]);
		current_len += max_len + 2;
		if(current_len + max_len + 2 > cols) {
			printf("\n");
			current_len = 0;
		}	
	}
	printf("\n");
} 

