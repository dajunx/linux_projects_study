#ifndef SHELL41_H
#define SHELL41_H

typedef struct Program{
        
        char **ppCmds; //command and args
        char **ppCmds_pipe; //command and args
       
        int iRedirectNum; //the length of the array
        
}Program_S;

Program_S  Create_Program(char *pCmds,int* has_pipe);//拆分命令行

void Print_Program(Program_S p);//打印内容 

void Creat_Exec1(Program_S p);//命令中无管道时执行

void Creat_Exec2(Program_S p);//命令中有管道时执行

void sig_usr(int signo);//信号量 
#endif
