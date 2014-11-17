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
	unsigned long fake_pgd;
	unsigned long addr;
	fd = open("/dev/zero",O_RDWR); 
	addr = (unsigned long)mmap(NULL,2048 * 512 * 4, PROT_READ, MAP_PRIVATE, fd, 0);
	fake_pgd = (unsigned long)malloc(2048 * 8);
	int pid  = atoi(argv[1]);
	int ret = syscall(378, pid, fake_pgd, addr);
	
	return 0;
}
