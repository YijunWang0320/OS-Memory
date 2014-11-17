#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>
#include <fcntl.h>
#define PAGE_SIZE 4096
#include <sys/mman.h>
int main(int argc, char *argv[])
{
	int fd;
	unsigned long *fake_pgd;
	fd = open("/dev/zero",O_RDWR); 
	fake_pgd = (unsigned long *)mmap(NULL, 2048 * 16, PROT_READ, MAP_SHARED, fd, 0);
	int pid  = atoi(argv[1]);
	int ret = syscall(378, pid, fake_pgd, 1);
	printf("fake_pgd : %lu", fake_pgd);

	int i;
	for(i = 0; i < 2048 * 16; i += 16)
	{
		printf("fake_pgd: %lu \n", *(fake_pgd + i));
	}
//	printf("%d",ret);
	return 0;
}
