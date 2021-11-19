#include <stdio.h>
#include <sys/stat.h>	// 这个头文件用来得到文件的详细信息
#include <string.h>
#include <time.h>	// 时间头文件
#include <pwd.h>	// 用来得到用户名
#include <grp.h>	// 用来得到组名
 
// 结构体,用来存储要输出的每个属性值
struct attribute
{
	char mode[10];		// 文件属性和权限
	int links;		// 链接数
	char user_name[20];	// 用户名
	char group_name[20];	// 所在的用户组
	long size;		// 文件大小
	char mtime[20];		// 最后修改的时间
	char filename[255];	// 文件名
	char extra[3];		// 用来显示时候要加 "*"(可以执行的文件) 或者 "/" (目录) 的额外字符串
};
 
// 计算整数 n 有几位
int f(long n) {
	int ret = 0;
	while(n) {
		n = n / 10;
		++ ret;
	}
	return ret;
}
 
// 得到终端的列数和行数
void getTerminatorSize(int *cols, int *lines) {
 
#ifdef TIOCGSIZE  
    struct ttysize ts;  
    ioctl(STDIN_FILENO, TIOCGSIZE, &ts);  
    *cols = ts.ts_cols;  
    *lines = ts.ts_lines;  
#elif defined(TIOCGWINSZ)  
    struct winsize ts;  
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);  
    *cols = ts.ws_col;  
    *lines = ts.ws_row;  
#endif /* TIOCGSIZE */  
 
}
 
// 由 int 型的 mode,得到实际要显示的字符串
void mode2str(int mode, char str[])
{
    strcpy(str, "----------\0"); 
    if(S_ISDIR(mode)) str[0] = 'd';
    if(S_ISCHR(mode)) str[0] = 'c';
    if(S_ISBLK(mode)) str[0] = 'b';
    if(S_ISLNK(mode)) str[0] = 'l';
 
    if(mode & S_IRUSR) str[1] = 'r';
    if(mode & S_IWUSR) str[2] = 'w';
    if(mode & S_IXUSR) str[3] = 'x';
 
    if(mode & S_IRGRP) str[4] = 'r';
    if(mode & S_IWGRP) str[5] = 'w';
    if(mode & S_IXGRP) str[6] = 'x';
 
    if(mode & S_IROTH) str[7] = 'r';
    if(mode & S_IWOTH) str[8] = 'w';
    if(mode & S_IXOTH) str[9] = 'x';
}
 
// 根据用户的 id 值,得到用户名
void uid2str(uid_t uid, char *user_name) /* 将uid转化成username */
{
	struct passwd *pw_ptr;
	pw_ptr = getpwuid(uid);
 
	if( pw_ptr == NULL) {
		sprintf(user_name, "%d", uid);
	} else {
		strcpy(user_name, pw_ptr->pw_name);
	}
}
 
// 根据用户组的 id 值,得到用户组名
void gid2str(gid_t gid, char *group_name) /* 将uid转化成username */
{
	struct group *grp_ptr;
	grp_ptr = getgrgid(gid);
 
	if( grp_ptr == NULL) {
		sprintf(group_name, "%d", gid);
	} else {
		strcpy(group_name, grp_ptr->gr_name);
	}
}
 
// 时间的格式化字符串, 注意这里我把前面的星期和后面的年份都去掉了
void time2str(time_t t, char *time_str)
{
	strcpy( time_str, ctime(&t) + 4);
	time_str[12] = '\0';
}
 
// 要显示的某一个文件详细信息,并把信息放在结构体 attribute 中
void ls_long_file(char *dirname, char *filename, struct attribute *file_attri) 
{
	// 根据文件夹名和文件名得到全名
	char fullname[256];
	strcpy(fullname, dirname);
	strcpy(fullname + strlen(dirname), filename);
 
	struct stat mystat;
	if ( stat(fullname, &mystat) == -1) {
		printf("ls_long_file: stat error\n");		
	} else {
		// 这里参考 <stat.h> 头文件
		int mode   = (int)  mystat.st_mode;
		int links  = (int)  mystat.st_nlink;
		int uid    = (int)  mystat.st_uid;
		int gid    = (int)  mystat.st_gid;
		long size  = (long) mystat.st_size;
		long mtime = (long) mystat.st_mtime;
 
		char str_mode[10];			/* 文件类型和许可权限, "drwxrwx---" */
		char str_user_name[20];
		char str_group_name[20];
		char str_mtime[20];
 
		// 这里就是直接调用上面的函数啦,洒家就不解释了
		mode2str(mode, str_mode);
		uid2str(uid, str_user_name);
		gid2str(gid, str_group_name);
		time2str(mtime, str_mtime);
		
		char extra[3] = "\0\0";
		if (str_mode[0] == 'd') {
			extra[0] = '/';
		} else if (str_mode[0] == '-' && str_mode[3] == 'x') {
			extra[0] = '*';
		}
			
		// 存储在结构体中
		strcpy(file_attri->mode, str_mode);
		file_attri->links = links;
		strcpy(file_attri->user_name, str_user_name);
		strcpy(file_attri->group_name, str_group_name);
		file_attri->size = size;
		strcpy(file_attri->mtime, str_mtime);
		strcpy(file_attri->filename, filename);
		strcpy(file_attri->extra, extra);
 
		// 大概要得到的效果就是下面的情况啦!
		/* 
			drwxr-xr-x  5 halfish halfish  4096 12月 28 10:35 Downloads/ 
		*/
	} 	
}

