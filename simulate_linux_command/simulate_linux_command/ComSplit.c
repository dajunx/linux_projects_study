#include"Myshell.h"

//字符串拆分函数(拆分命令)
int split_input_cmd(char str_cmds[N], int time) {
  char current_path[N];//用于存放获取的路径
  int i = 0, word = 0, m = 0;
  //记录字符命令的组数，即";" 分号分隔的数目
  int iCmds_array = 0;
  
  //对输入的命令行进行统计: 1.命令的个数 2.每个命令的长度
  for (i = 0; str_cmds[i] != '\0'; i++) {
    if (str_cmds[i] == ';') { //遇到第一个空格，去掉连续的多个空格
      word = 0;
    } else if (word == 0) { //空格为界统计命令的个数
      word = 1;
      iCmds_array++;//统计命令组数,用于给p分配空间
    }
  }
  
  getcwd(current_path, sizeof(current_path)); //获取当前路径
  cmds = (char**)malloc((iCmds_array + 1) * sizeof(char*));
  
  //拆分命令行，以分号作为拆分的标志
  m = 0;
  cmds[m] = strtok(str_cmds, ";");
  while (cmds[m] != NULL) {
    cmds[++m] = strtok(NULL, ";");
  }
  
  has_pipe = 0;//初始化标记符
  
  //对以分号进行拆分后的命令继续拆分
  for (i = 0; i < iCmds_array; i++) {
    if ((int)cmds[i] == 0) { 
		//避免当p为空时进行非法操作
		continue; 
	}
    
    has_pipe = pipe_judge(cmds[i]); //判断是否有管道
    //调用拆分函数以管道符号进行拆分,若没有管道将其存入str_q中
	split_with_pipe(i);
    m = 0;
    /*
    若没有管道的情况就以空格拆了后放在str_p中
    */
	//以空格作为拆分的标志
    cmds_after_space[m] = strtok(cmds_after_pipe[0], " ");
    
    while (cmds_after_space[m]) { //将拆分后的字符放在str_p[]中
      cmds_after_space[++m] = strtok(NULL, " ");
    }
    
    m = 0;
    /*
    有管道就将管道后面的命令以空格放在拆了后放在str_h中
    */
    str_h[m] = strtok(cmds_after_pipe[1], " ");//以空格作为拆分的标志
    
    while (str_h[m]) { //将拆分后的字符放在str_h[]中
      str_h[++m] = strtok(NULL, " ");
    }
    
    handle_inner_cmd(current_path, time);
  }
  
  return 0;
}

// 校验命令行中是否含有管道
int pipe_judge(char buf[N]) {
  int i;
  has_pipe = 0; //0 表示不含有管道
  
  for (i = 0; buf[i] != '\0'; i++) {
    if (buf[i] == '|') {
      has_pipe = 1;
	  break;
    }
  }
  
  return has_pipe;
}

// 对命令以管道进行拆分
int split_with_pipe(int i) {
  int m = 0;
  cmds_after_pipe[m] = strtok(cmds[i], "|");
  
  while (cmds_after_pipe[m]) {
    cmds_after_pipe[++m] = strtok(NULL, "|");
  }
  
  return 0;
}
