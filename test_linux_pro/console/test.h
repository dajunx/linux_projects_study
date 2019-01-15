#include<stdio.h>
int add()
{
	int i;
	for(i=0;i<10;i++)
	{
		sleep(1);
		printf("i=%d\n",i);
	}
}