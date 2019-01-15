#include"Myshell.h"

//内部命令处理函数
int handle_inner_cmd(char route[N], int time) {
  char tempRoute[N];
  
  if (strcmp(cmds_after_space[0], "cd") == 0) { //切换目录
    if (time == 1) { //第一次运行时将当前路径保存在中间数组中
      strcpy(route_old, route);
    } else {
      //将第一次运行时保存的路径给route_old
      strcpy(tempRoute, route_old);
      //将当前路径保存在中间数组中
      strcpy(route_old, route);
    }
    
    if (cmds_after_space[1] == NULL) {
	  //如果所输的命令cd且后面没有参数，切换到根目录
      chdir(getenv("HOME"));
    } else if (strcmp(cmds_after_space[1], "-") != 0) {
	  //cd加参数（切换到某个指定目录）
      chdir(cmds_after_space[1]);
    } else {
	  //切换到上一次路径
      printf("%s\n", tempRoute);
      chdir(tempRoute);
    }
  } else if (strcmp(cmds_after_space[0], "exit") == 0) {
	  //退出
    exit(0);
  } else {
	  //外部命令，调用外部命令函数
	  external_cmd_handle(); 
  }
  
  return 0;
}

//命令处理函数
int cmd_mng() {
  char str_cmds[N];//存放路径的一个中间变量
  char ch[N];
  int time = 1; //time=1表示是第一次执行
  
  while (1) {
	  //命令提示符
    cmd_display();

    fgets(str_cmds, sizeof(str_cmds), stdin); //存放输入字符的数组
    fflush(stdin);
    str_cmds[strlen(str_cmds) - 1] = '\0';
    strcpy(ch, str_cmds);
    
    if (!strtok(ch, " ")) {
      continue;
    }
    
	//字符串拆分
    split_input_cmd(str_cmds, time);
    time++;//计数器自增
  }
  
  return 0;
}

