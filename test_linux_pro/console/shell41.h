#ifndef SHELL41_H
#define SHELL41_H

typedef struct Program{
        
        char **ppCmds; //command and args
        char **ppCmds_pipe; //command and args
       
        int iRedirectNum; //the length of the array
        
}Program_S;

Program_S  Create_Program(char *pCmds,int* has_pipe);//���������

void Print_Program(Program_S p);//��ӡ���� 

void Creat_Exec1(Program_S p);//�������޹ܵ�ʱִ��

void Creat_Exec2(Program_S p);//�������йܵ�ʱִ��

void sig_usr(int signo);//�ź��� 
#endif
