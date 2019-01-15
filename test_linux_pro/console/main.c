#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include"shell41.h"
int main()
{
        //Printf(); 
     	if(signal(SIGINT,sig_usr)==SIG_ERR)	//To terminate the program
        	printf("can't carry SIGINT");

        if(signal(SIGTSTP,sig_usr)==SIG_ERR)	//To pause the program
                printf("can't carry SIGTSTP");
 		
	 while(1){
                char ch[20];
                sleep(1);
                memset(ch,0,sizeof(ch));
                
        	Printf(); 
                scanf("%[^\n]",ch);
                getchar();
                if(strcmp("quit",ch)==0) {
                  exit(0);
		} else if(strcmp(ch,"")==0) {
		  printf("input data is empty\n");
		  break;
		}
                int has_pipe = 0;
                Program_S P=Create_Program(ch, &has_pipe);
                Print_Program(P);

                if (!has_pipe) {
                  Creat_Exec1(P); 
                } else {
                  Creat_Exec2(P);
                }
        }
	return 0;
}
