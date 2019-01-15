#include"Myshell.h"

//外部命令处理函数
int external_cmd_handle() {
  pid_t pid = fork();//创建子进程
  int status = 0;//用于父进程h回收子进程资源
  
  if (pid < 0) {
    //创建失败
    printf("fork error");
  } else if (pid == 0) {
    //子进程入口
    signal(SIGINT, SIG_DFL);//将中断信号忽略，返回命令提示符
    signal(SIGSTOP, SIG_DFL);
    
    if (has_pipe == 1) { //是管道的情况
      do_pipe();//执行管道处理函数
    } else {
      //直接执行外部命令
      if (execvp(cmds_after_space[0], cmds_after_space) == -1) {
        printf("failed!\n");
      }
      
      exit(0);
    }
    
    exit(0);//子进程退出
  } else {
    //父进程回收子进程资源
    wait(&status);
  }
  
  return 0;
}

int do_pipe() { //管道命令处理函数
  int pid, i, ret;
  int fd[2]; // 用于标记创建管道时返回的文件描述符
  
  if ((ret = pipe(fd)) == -1) {
    perror("pipe error"); //创建管道失败
    exit(1);
  }
  
  // 创建两个子进程，一个用于写管道，一个进行读管道
  for (i = 0; i < 2; i++) {
    if ((pid = fork()) == 0) {
      break;
    } else if (pid == -1) {
      perror("pipe error");//创建失败
      exit(1);
    }
  }
  
  if (i == 0) {
    // 关闭读管道端，打开写管端，子进程0进行写管道
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    execvp(cmds_after_space[0], cmds_after_space);//系统调用实现外部命令
  } else if (i == 1) {
    //关闭写管道端，打开读管端，子进程1进行写管道
    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    execvp(str_h[0], str_h); //系统调用实现外部命令
  } else {
    //父进程关闭写端和读端，回收子进程资源
    close(fd[0]);
    close(fd[1]);
    
	//两个儿子wait两次
    for (i = 0; i < 2; i++) {
      wait(NULL);
    }
  }
  
  return 0;
}
