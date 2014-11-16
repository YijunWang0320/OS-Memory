#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>
int main(int argc, char *argv[])
{
	int pid  = atoi(argv[1]);
	int ret = syscall(378,pid,1,1);
	printf("%d",ret);
	return 0;
}
