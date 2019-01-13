#include"Myshell.h"

char **cmds = NULL;//用于存放拆分后的命令
char *cmds_after_space[N] = {NULL};
char *str_h[N] = {NULL};
char *cmds_after_pipe[N] = {NULL};
char route_old[N];//存放上一次路径
int has_pipe=0;

int main()
{
	signal(SIGTSTP, SIG_DFL);//建立暂停信号，设置为默认信号
	signal(SIGCONT,SIG_DFL);//建立恢复暂停信号，设置为默认信号
	signal(SIGINT,SIG_IGN);//建立中断信号，并设置为可忽略的信号
	
	cmd_mng();//内部命令处理函数

	free(cmds);//释放用于存放拆分后的命令的数组
	return 1;		
}
