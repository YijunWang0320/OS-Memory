#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int main()
{
	long pid;
	int ret = syscall(378,50,1,1);
	printf("%d",ret);
	return 0;
}
