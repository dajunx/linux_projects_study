#include <arpa/inet.h>  /* inet(3) functions */
#include <netdb.h>      /*gethostbyname function */
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

//------------------- function define ----------------------------------
void trans_file_by_socket(int sockfd);

int main() {
  char *servInetAddr = (char *)malloc(10);
  sprintf(servInetAddr, "%s", "119.29.36.228");
  int servPort = 6888;
  int connfd;
  struct sockaddr_in servaddr;

  connfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(servPort);
  inet_pton(AF_INET, servInetAddr, &servaddr.sin_addr);

  int connect_server_ret;
  do
  {
    sleep(1);
    connect_server_ret = connect(connfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("connect_server_ret: %d\n", connect_server_ret);
  } while (connect_server_ret < 0);

  trans_file_by_socket(connfd); /* do it all */
  close(connfd);
  printf("exit\n");

  return 0;
}

// 解析本地文件，然后使用网络模块发送到服务端
#define MAXSIZE 1500
void trans_file_by_socket(int sockfd) {
  std::string source_file_name("./text_file");

  // 打开指定文件
  int open_fd_src = open(source_file_name.c_str(), O_RDONLY);
  if (open_fd_src == -1) {
    std::cout << "open " << source_file_name 
              << " file failed, errno: "<< strerror(errno) << std::endl;
    return;
  }

  // 分块解析打开的文件
  char buf[MAXSIZE];
  size_t read_bytes = -1;
  size_t api_ret;
  int package_count = 0;
  while ((read_bytes = read(open_fd_src, buf, MAXSIZE-1)) > 0) {
    // 使用网络接口传输读取到的数据
    api_ret = write(sockfd, buf, read_bytes);
    if (api_ret == -1) {
      std::cout << "write data to socket meet err, errno:" 
                << strerror(errno) << std::endl;
      continue;
    }
    package_count++;
    ///TODO 等待对端应答
    //api_ret = read(sockfd, recvline, MAXLINE);
  }
  std::cout << "client send package count: " << package_count << std::endl;
}