#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
#include<signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>


#define N 50 //用于给各数组分配空间

extern int has_pipe;//用于标记管道
extern char** cmds ;//用于存放以分号拆分后的命令
extern char* cmds_after_space[N];//用于存放以空格拆分后的命令
extern char* str_h[N];//用于存放拆空格分后的命令
extern char* cmds_after_pipe[N];//用于存放拆管道分后的命令
extern char route_old[];//存放上一次路径

int split_input_cmd(char str[N],int time);//字符串拆分函数
int cmd_display();//命令行提示符函数
int cmd_mng();//命令处理函数
int external_cmd_handle();//外部命令函数
int handle_inner_cmd(char route[N],int time);//内部命令处理函数
int do_pipe();//管道处理函数
int pipe_judge(char buf[N]);//判断是否含有管道符
int split_with_pipe(int i);
