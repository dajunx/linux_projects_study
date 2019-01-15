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

////��������У��Կո�����
Program_S  Create_Program(char* pCmds, int* has_pipe) {
  Program_S r;
  int i, j = 0;
  int pipe_index = 0; // // ���ڼ�¼pipe�ָ���"|"����
  int index[50];  //  // ������¼ÿ�������ֶε�ƫ��λ��
  
  if (pCmds[0] != ' ') {
    index[j++] = 0;
  }
  
    /**
   * �������������ÿ���ַ��������²�����
   * 1.����"|" �ܵ�����������¼���ַ�λ�ã������Ϻ��йܵ����������
   * 2.����" " �ַ����û���0�� ��ʹ��index ��¼һ��ÿ������������߲�����ƫ������
   * 3.���������ַ���������ѭ�������һ���ַ�
   */ 
  for (i = 0; pCmds[i];) {
    if (pCmds[i] == '|') {
      //  // �ҵ��˹ܵ����ţ��� has_pipe���ϱ�ǣ��������������йܵ�����
      *has_pipe = 1;
      pipe_index = i++;
      continue;
    } else if (pCmds[i] != ' ') {
      i++;
      continue;
    }
    
    while (pCmds[i] == ' ') {
      pCmds[i++] = 0;//���ָ���ֹԽ�� 
    }
    
    index[j++] = i;
  }
  
  r.iRedirectNum = j;

  // ��������������ڴ�
  r.ppCmds = (char**)malloc((j + 1) * sizeof(char*));
  if (*has_pipe) {
    r.ppCmds_pipe = (char**)malloc((j + 1) * sizeof(char*));
  }
  
  //  �ֱ��¼�ܵ�ǰ������Ͳ��������� �ܵ����������߲�������
  int normal_cmd_count = 0, pipe_cmd_count = 0;
  
  for (i = 0; i < j; i++) {
    if (pipe_index != 0 && pipe_index == index[i]) {
      // �ܵ���������ִ�������ʱ����Ҫ��������?      continue;
    } else if (pipe_index != 0 && pipe_index < index[i]) {
      // �ܵ����������߲���
      r.ppCmds_pipe[pipe_cmd_count++] = pCmds + index[i];
    } else {
      // �ܵ�ǰ��������߲���
      r.ppCmds[normal_cmd_count++] = pCmds + index[i];
    }
  }
  
  // ��"|"�ܵ�֮ǰ������ ��ӽ���
  r.ppCmds[normal_cmd_count] = 0;
  
  if (*has_pipe) {
    // ��"|"�ܵ�֮������� ��ӽ���
    r.ppCmds_pipe[pipe_cmd_count] = 0;
  }
  
  return r;
}

void Print_Program(Program_S p) {
  int i = 0;
  
  while (p.ppCmds[i] != NULL) { //��ӡ���������
    printf("%s\n", p.ppCmds[i]);
    i++;
  }
}

// �������޹ܵ�������ִ�����µ���
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

// �������йܵ�������ִ�����µ���
void Creat_Exec2(Program_S p) {
  int status = 0, fd[2], pid, i = 0;
  pipe(fd);
  
  //  ���������̣߳�һ���߳�ִ�йܵ�ǰ�����һ��ִ�йܵ������������
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
    
    // ���̵߳ȴ�����io�߳�ִ������ٷ���
    for (i = 0; i < 2; i++) {
      wait(NULL);
    }
  }
}


int paused = 0;//������Ʊ��� 
void sig_usr(int signo) {
  if (signo == SIGINT) { //To terminate the program
    printf("received SIGINT\n");
    exit(0);//������ctrl+C���ź�SIGINT��pasue��1��ִ���жϲ��� 
  } else if (signo == SIGTSTP) { //To pause the program
    printf("received SIGTSTP\n");
    paused ^= 1;
    
    while (paused) { sleep(1); }//������ctrl+Z���ź�SIGTSTP��pasue��1��ִ�й������ 
  } else { //Beside,original output
    printf("received signal %d\n", signo);
  }
}

void Printf() {
  char  strl[100], host [50], buf[50]/*, path[50] = {0}*/;
  char path[255];
  gethostname(host, sizeof(host));//��ȡ������ 
  //getcwd(strl, 100);
  //getcwd(buf, 50);
  getcwd(path, 255);//��ȡ�ļ�·�� 
  printf("[wumeixue@");//�����û��� 
  printf("%s", host);//��������� 
  //printf("@");
  printf(" %s", path);//���·�� 
  printf("]$ ") ;
}


