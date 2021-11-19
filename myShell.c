#include<stdio.h>                                                                                                                       
#include<unistd.h>
#include<stdlib.h>
#include<wait.h>
#include<string.h>
#include<pwd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

//用于将路径进行切分，最后一个/后面为当前路径
void Parse(char* input, char* output[])
{
    char* p = strtok(input,"/");
    int i = 0;
    while(p!= NULL)
    {
        output[i++] = p;
        p=strtok(NULL,"/");
    }
}

//用于对输入的命令进行切分
void ParseArg(char* input, char* output[])  
{
    char* p = strtok(input," ");
    int output_size = 0;
    while(p != NULL)
    {
        output[output_size++] = p;
        p = strtok(NULL," ");
    }
    output[output_size] = NULL;
}

void Exec(char* argv[])  //创建子进程执行进程程序替换
{
    pid_t pid = fork();
    if(pid == -1)                                                                                                                       
    {
        perror("fork()");
    }
    else if(pid == 0)
    {
         //child                                                                                                                         
        execvp(argv[0],argv);
        //execvp失败会执行下面的语句
        perror("execvp()");
        exit(0);
    }
    else
    {
        //parent
        wait(NULL);
    }
}

//判断指令是否包含管道
int IsPipe(char* argv[])
{
    int i = 0;
    while(argv[i] != NULL)
    {
        if(strcmp(argv[i], "|") == 0)
        {
            return i+1; //i是管道，则i+1就是管道的下一个命令
        }
        ++i;
    }
    return 0;
}

void ParsePipe(char* input[], char* output1[],char* output2[])//用于将input按照|进行切分，最后一个后面为当前路径                    

{
    int i = 0;
    int size1 = 0;
    int size2 = 0;                                                                                                                      
    int ret = IsPipe(input);

    while(strcmp(input[i], "|")!=0)
    {
        output1[size1++] = input[i++];
    }
     output1[size1] = NULL;

     int j = ret;//j指向管道的后面那个字符
     while(input[j] != NULL)
     {
         output2[size2++] = input[j++];
     }

     output2[size2] = NULL;
}


void ExecvPipe(char* argv1[],char* argv2[])
{
    pid_t pid = fork();
    if(pid == -1)
    {                                                                                                                           
        perror("fork()");
    }                                                                                                                                   
    else if(pid == 0)
    {
        //child,创建一个管道
        int fd[2];
        int ret = pipe(fd);
        if(fork() == 0 )
        {
            //子进程创建一个子进程，用于执行管道之前的命令
            close(fd[0]); //孙子进程关闭掉读端
            dup2(fd[1],1);  //将标准输出作为管道的标准输入
            execvp(argv1[0],argv1);
        }
        wait(NULL);

        close(fd[1]); //关闭掉写端
        dup2(fd[0],0);
        execvp(argv2[0],argv2);
        //execvp失败会执行下面的语句
        perror("execvp()");
        exit(0);    
    }
    else                                                                                                                                
    {
        //parent
        wait(NULL);
    }
}
int main()
{
    //获得当前路径
    while(1)
    {
        //首先显示shell提示
        //通过getuid获得用户的id,然后通过getpwuid通过用户的id查找用户的passwd数据来获取系统登录的用户名

        //获得当前路径
        char path[100] = {0};
        getcwd(path,sizeof(path)); //getcwd获得的是绝对路径(path里面存放的是绝对路径)
        char* path2[100] = {0};                                                                                                             
        Parse(path,path2);

        char* path3 = NULL;  //path里面为绝对路径，将path按照/进行分割，分割的结果保存在path2里面,path3里面保存最后一个字符串，即相对路>径                                                                                                                                      
        int i = 0;
        while(path2[i] !=NULL)
        {
            path3= path2[i];
            i++;
        }      

        struct passwd *pwd;
        pwd = getpwuid(getuid());
        //获得主机名
        char hostname[100] ={0};
        gethostname(hostname,sizeof(hostname));
        printf("[%s@%s %s MyShell]",pwd->pw_name,hostname,path3);
        fflush(stdout);

        //从标准输入读取字符串
        char buf[1024] = {0};
        gets(buf);

        //解析字符串，解析出指令和参数
        char* argv[100];
        ParseArg(buf,argv);

        //判断解析出来的字符串数组里面是否含有管道                                                                                      
        int ret1 = IsPipe(argv);
        if(ret1 > 0)
        {
            //是管道
            char* argv1[10];
            char* argv2[10];
            ParsePipe(argv,argv1,argv2);
            ExecvPipe(argv1,argv2);
        }
        else
        {
            Exec(argv);
        }
    }
    return 0;
}
