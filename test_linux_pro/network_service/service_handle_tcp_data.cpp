#include <arpa/inet.h>  /* inet(3) functions */
#include <netinet/in.h> /* sockaddr_in{} and other Internet defns */
#include <sys/socket.h> /* basic socket definitions */
#include <sys/types.h>  /* basic system data types */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

#define MAXLINE 1024
void handle_network_data_by_socket(int connfd);

int main() {
  int listenfd, connfd;
  int serverPort = 6888;
  int listenq = 1024;
  pid_t child_pid;
  socklen_t socklen;

  struct sockaddr_in cliaddr, servaddr;
  socklen = sizeof(cliaddr);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(serverPort);

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    perror("socket error");
    return -1;
  }

  // 处理 address already in use. 错误
  int enable = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) <
    0) {
    perror("setsockopt(SO_REUSEADDR) failed");
    return -1;
  }

  if (bind(listenfd, (struct sockaddr *)&servaddr, socklen) < 0) {
    perror("bind error");
    return -1;
  }
  if (listen(listenfd, listenq) < 0) {
    perror("listen error");
    return -1;
  }
  printf("echo server startup,listen on port:%d\n", serverPort);
  for (;;) {
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &socklen);
    if (connfd < 0) {
      perror("accept error");
      continue;
    }

    printf("accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr),
      cliaddr.sin_port);

    child_pid = fork();
    if (child_pid == 0) { //child process
      //close(listenfd); // close listening socket ??? 为啥要关闭掉
      handle_network_data_by_socket(connfd);
      exit(0);
    }
    else if (child_pid > 0) {
      // parent process
      //close(connfd); // closes connected socket ??? 为啥要关闭掉
    }
    else {
      perror("fork error");
    }
  }

  return 0;
}

// 解析本地文件，然后使用网络模块发送到服务端
#define MAXSIZE 200
void handle_network_data_by_socket(int sockfd) {
  std::string source_file_name("./text_file1");

  // 打开指定文件
  int openFlags, filePerms;
  openFlags = O_CREAT | O_WRONLY /*| O_TRUNC*/;
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
              S_IROTH | S_IWOTH;      /* rw-rw-rw- */
  int open_fd_dsc = open(source_file_name.c_str(), openFlags, filePerms);
  if (open_fd_dsc == -1) {
    std::cout << "open " << source_file_name 
              << " file failed, errno: " << strerror(errno) << std::endl;
    return;
  }

  // 分块接收发过来的数据，并写入本地文件
  char buf[MAXSIZE];
  size_t read_bytes = -1;
  size_t api_ret;
  std::cout << "begin receive data."<< std::endl;
  int package_count = 0;
  while ((read_bytes = read(sockfd, buf, MAXSIZE-1)) > 0) {
    // 使用网络接口传输读取到的数据
    api_ret = write(open_fd_dsc, buf, read_bytes);
    if (api_ret == -1) {
      std::cout << "write data to socket meet err, errno: " 
                << strerror(errno) << std::endl;
      continue;
    }
    package_count++;
  }
  std::cout << "finished receive data, receive package count: " 
            << package_count << std::endl;
}