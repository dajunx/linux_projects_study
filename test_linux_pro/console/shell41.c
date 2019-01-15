#include<stdio.h>
#include<ctype.h>
#include<fcntl.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include <stdbool.h>
#include"shell41.h"

////拆分命令行，以空格区分
Program_S  Create_Program(char* pCmds, int* has_pipe) {
  Program_S r;
  int i, j = 0;
  int pipe_index = 0; // // 用于记录pipe分隔符"|"索引
  int index[50];  //  // 用来记录每个命令字段的偏移位置
  
  if (pCmds[0] != ' ') {
    index[j++] = 0;
  }
  
    /**
   * 遍历输入命令的每个字符，做如下操作：
   * 1.遇到"|" 管道操作符，记录该字符位置，并打上含有管道操作符标记
   * 2.遇到" " 字符，置换成0， 并使用index 记录一下每个操作命令或者参数的偏移量；
   * 3.遇到其他字符，跳过，循环检查下一个字符
   */ 
  for (i = 0; pCmds[i];) {
    if (pCmds[i] == '|') {
      //  // 找到了管道符号，给 has_pipe打上标记，表明输入的命令含有管道操作
      *has_pipe = 1;
      pipe_index = i++;
      continue;
    } else if (pCmds[i] != ' ') {
      i++;
      continue;
    }
    
    while (pCmds[i] == ' ') {
      pCmds[i++] = 0;//设空指针防止越界 
    }
    
    index[j++] = i;
  }
  
  r.iRedirectNum = j;

  // 给缓存命令分配内存
  r.ppCmds = (char**)malloc((j + 1) * sizeof(char*));
  if (*has_pipe) {
    r.ppCmds_pipe = (char**)malloc((j + 1) * sizeof(char*));
  }
  
  //  分别记录管道前的命令和参数个数， 管道后的命令或者参数个数
  int normal_cmd_count = 0, pipe_cmd_count = 0;
  
  for (i = 0; i < j; i++) {
    if (pipe_index != 0 && pipe_index == index[i]) {
      // 管道操作符，执行命令的时候不需要，故跳过?      continue;
    } else if (pipe_index != 0 && pipe_index < index[i]) {
      // 管道后的命令或者参数
      r.ppCmds_pipe[pipe_cmd_count++] = pCmds + index[i];
    } else {
      // 管道前的命令或者参数
      r.ppCmds[normal_cmd_count++] = pCmds + index[i];
    }
  }
  
  // 给"|"管道之前的命令 添加结束
  r.ppCmds[normal_cmd_count] = 0;
  
  if (*has_pipe) {
    // 给"|"管道之后的命令 添加结束
    r.ppCmds_pipe[pipe_cmd_count] = 0;
  }
  
  return r;
}

void Print_Program(Program_S p) {
  int i = 0;
  
  while (p.ppCmds[i] != NULL) { //打印输入的命令
    printf("%s\n", p.ppCmds[i]);
    i++;
  }
}

// 命令中无管道操作，执行如下调用
void Creat_Exec1(Program_S p) {
  pid_t pid = fork();
  
  if (pid < 0) {
    printf("fork error!");
  } else if (pid == 0) {
    if (execvp(p.ppCmds[0], p.ppCmds) < 0) {
      printf("exec error!");
      exit(1);
    }
  } else {
    wait(NULL);
  }
}

// 命令中有管道操作，执行如下调用
void Creat_Exec2(Program_S p) {
  int status = 0, fd[2], pid, i = 0;
  pipe(fd);
  
  //  启动两个线程，一个线程执行管道前的命令，一个执行管道操作后的命令
  for (i = 0; i < 2; i++) {
    if ((pid = fork()) == 0) { break; }
  }
  
  if (i == 0) {
    close(0);
    dup(fd[0]);
    close(fd[1]);
    execvp(p.ppCmds_pipe[0], p.ppCmds_pipe);
  } else if (i == 1) {
    close(1);
    dup(fd[1]);
    close(fd[0]);
    execvp(p.ppCmds[0], p.ppCmds);
  } else {
    close(fd[0]);
    close(fd[1]);
    
    // 主线程等待所有io线程执行完后再返回
    for (i = 0; i < 2; i++) {
      wait(NULL);
    }
  }
}


int paused = 0;//定义控制变量 
void sig_usr(int signo) {
  if (signo == SIGINT) { //To terminate the program
    printf("received SIGINT\n");
    exit(0);//若输入ctrl+C测信号SIGINT，pasue置1，执行中断操作 
  } else if (signo == SIGTSTP) { //To pause the program
    printf("received SIGTSTP\n");
    paused ^= 1;
    
    while (paused) { sleep(1); }//若输入ctrl+Z测信号SIGTSTP，pasue置1，执行挂起操作 
  } else { //Beside,original output
    printf("received signal %d\n", signo);
  }
}

void Printf() {
  char  strl[100], host [50], buf[50]/*, path[50] = {0}*/;
  char path[255];
  gethostname(host, sizeof(host));//获取主机名 
  //getcwd(strl, 100);
  //getcwd(buf, 50);
  getcwd(path, 255);//获取文件路径 
  printf("[wumeixue@");//定义用户名 
  printf("%s", host);//输出主机名 
  //printf("@");
  printf(" %s", path);//输出路径 
  printf("]$ ") ;
}


