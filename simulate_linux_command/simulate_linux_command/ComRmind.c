#include"Myshell.h"

int cmd_display() { //命令行提示符函数
  int i = 0, j = 0;
  char route_temp[N], host_name[100], s[N];
  /*route_temp用于存放当前路径，host_name用于存放当前主机类型，s作为中间辅助数组*/
  //获取当前路径
  getcwd(route_temp, sizeof(route_temp));
  /*将当前主机类型复制给host_name，用于对其截取前半段*/
  //strcpy(host_name, getenv("HOSTNAME")); // 修改
  
  //以‘.’为分界截取主机前半段
  for (i = 0, j = 0; host_name[i] != '.'; i++, j++) {
    host_name[j] = host_name[i];
  }
  
  host_name[j] = '\0';
  
  //以‘/’为分界反向截取当前路径
  for (i = (int)(strlen(route_temp) - 1), j = 0; route_temp[i] != '/'; i--, j++) {
    s[j] = route_temp[i];
  }
  
  s[j] = '\0';
  
  //将截取后的字符倒过来放在用于存放当前路径的数组中
  for (i = (int)(strlen(s) - 1), j = 0; i >= 0; i--, j++) {
    route_temp[j] = s[i];
  }
  
  route_temp[j] = '\0';
  //输出命令提示符
  printf("[zhangyuanyi@%s %s]$", host_name, route_temp);

  return 0;
}

